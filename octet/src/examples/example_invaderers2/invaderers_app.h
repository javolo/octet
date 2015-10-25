////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// invaderer example: simple game with sprites and sounds
//
// Level: 1
//
// Demonstrates:
//   Basic framework app
//   Shaders
//   Basic Matrices
//   Simple game mechanics
//   Texture loaded from GIF file
//   Audio
//

namespace octet {
  class sprite {
    // where is our sprite (overkill for a 2D game!)
    mat4t modelToWorld;

    // half the width of the sprite
    float halfWidth;

   

    // what texture is on our sprite
    int texture;

    // true if this sprite is enabled.
	bool enabled;


  public:

	// half the height of the sprite
	float halfHeight;

    sprite() {
      texture = 0;
      enabled = true;
    }

    void init(int _texture, float x, float y, float w, float h) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      halfWidth = w * 0.5f;
      halfHeight = h * 0.5f;
      texture = _texture;
      enabled = true;
    }

	// Return the sprite position
	vec2 get_position(){
		return modelToWorld.row(3).xy();
	}

	// Rotate sprite some specific angle
	void rotate_sprite_y(float angle){
		modelToWorld.rotateY(angle);
	}


    void render(texture_shader &shader, mat4t &cameraToWorld) {
      // invisible sprite... used for gameplay.
      if (!texture) return;

      // build a projection matrix: model -> world -> camera -> projection
      // the projection space is the cube -1 <= x/w, y/w, z/w <= 1
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      // set up opengl to draw textured triangles using sampler 0 (GL_TEXTURE0)
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      // use "old skool" rendering
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      shader.render(modelToProjection, 0);

      // this is an array of the positions of the corners of the sprite in 3D
      // a straight "float" here means this array is being generated here at runtime.
      float vertices[] = {
        -halfWidth, -halfHeight, 0,
         halfWidth, -halfHeight, 0,
         halfWidth,  halfHeight, 0,
        -halfWidth,  halfHeight, 0,
      };

      // attribute_pos (=0) is position of each corner
      // each corner has 3 floats (x, y, z)
      // there is no gap between the 3 floats and hence the stride is 3*sizeof(float)
      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)vertices );
      glEnableVertexAttribArray(attribute_pos);
    
      // this is an array of the positions of the corners of the texture in 2D
      static const float uvs[] = {
         0,  0,
         1,  0,
         1,  1,
         0,  1,
      };

      // attribute_uv is position in the texture of each corner
      // each corner (vertex) has 2 floats (x, y)
      // there is no gap between the 2 floats and hence the stride is 2*sizeof(float)
      glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)uvs );
      glEnableVertexAttribArray(attribute_uv);
    
      // finally, draw the sprite (4 vertices)
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // move the object
    void translate(float x, float y) {
      modelToWorld.translate(x, y, 0);
    }

    // position the object relative to another.
    void set_relative(sprite &rhs, float x, float y) {
      modelToWorld = rhs.modelToWorld;
      modelToWorld.translate(x, y, 0);
    }

    // return true if this sprite collides with another.
    // note the "const"s which say we do not modify either sprite
    bool collides_with(const sprite &rhs) const {
	  float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
	  float dy = rhs.modelToWorld[3][1] - modelToWorld[3][1];

      // both distances have to be under the sum of the halfwidths
      // for a collision
	  return
		(fabsf(dx) < halfWidth + rhs.halfWidth) &&
		(fabsf(dy) < halfHeight + rhs.halfHeight)
	  ;
    }

    bool is_above(const sprite &rhs, float margin) const {
      float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];

      return
        (fabsf(dx) < halfWidth + margin)
      ;
    }

    bool &is_enabled() {
      return enabled;
    }
  };

  class invaderers_app : public octet::app {
    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;

    // shader to draw a textured triangle
    texture_shader texture_shader_;

	// Definition of gravity constant
	const float gravity = 9.8f;
	// We set a maximum height constant
	const float max_height = 0.2f;
	// When we reach the max_height we want to touch the floor again before jump again
	bool player_descending = false;
	// We set a variable to move score and background images
	float num_movement = 1.0;
	float position_score;
	// We set up the a constant width variable
	const float window_width = 6.0f;
	// We set the counter for update coin position
	int counter_coin = 1;

    enum {
	  // Constants definition
      num_borders = 4,

      // sprite definitions
      ship_sprite = 0,
      game_over_sprite,

      first_border_sprite,
      last_border_sprite = first_border_sprite + num_borders - 1,

	  background_sprite,
	  background_sprite1,
	  coin_sprite,
	  player_sprite,

      num_sprites,

    };

    // Number of lives variable
    int num_lives;

    // game state
    bool game_over;
    int score;

    // Speed of the player
    float player_velocity;

    // big array of sprites
    sprite sprites[num_sprites];

    // random number generator
    class random randomizer;

    // a texture for our text
    GLuint font_texture;

    // information for our text
    bitmap_font font;

    // Use the keyboard to move the player around the screen
    void move_player() {
      float player_speed = 0.05f;
      // left and right arrows
      if (is_key_down(key_left)) {
		  sprites[player_sprite].translate(-player_speed, 0);
		  // We move the camera with the player and the score
		  cameraToWorld.translate(-player_speed, 0, 0);
		  // We move the bottom border with the camera movement
		  sprites[first_border_sprite + 0].translate(-player_speed, 0);
		  if (sprites[player_sprite].collides_with(sprites[first_border_sprite+2])) {
			  sprites[player_sprite].translate(+player_speed, 0);
			  cameraToWorld.translate(+player_speed, 0, 0);
        }
      } 
	  if (is_key_down(key_right)) {
		  sprites[player_sprite].translate(+player_speed, 0);
		  // We move the camera with the player and the score
		  cameraToWorld.translate(+player_speed, 0, 0);
		  // We move the bottom border with the camera movement
		  sprites[first_border_sprite + 0].translate(+player_speed, 0);
		  //printf("CAM X: %f \n", cameraToWorld.colx().length());
		  // We need to check if the camera fits with the second background to move the first one
		  if (cameraToWorld.row(3).x()>(num_movement*window_width)){
			  // We know that 6.0f is the width of the window.
			  // We find out which background we have to move
			  int num = cameraToWorld.row(3).x() / window_width;
			  if (num % 2 == 0){
				  sprites[background_sprite + 1].translate(+(2 * window_width), 0);
			  } else {
				  sprites[background_sprite + 0].translate(+(2 * window_width), 0);
			  }
			  // We move the left border as well because if there isn´t background image any more if we move left
			  sprites[first_border_sprite + 2].translate(+window_width, 0);
			  num_movement++;
		  }
	  } 
	  if (is_key_down(key_up)) {
		  // We check if the player is descending to see if he can start the jump again 
		  // or has to wait until the previous jump has finished
		  if (!player_descending){
			  // We add some force to the jump to make the effect
			  player_speed += 0.30f;
			  sprites[player_sprite].translate(0, +player_speed);
			  // We check if we have reached the maximum height to start pushing the player down
			  if (sprites[player_sprite].get_position().y() > max_height){
				  // Update boolean variable in case we left pressed the up button
				  player_descending = true;
				  sprites[player_sprite].translate(0, -player_speed);
				  if (sprites[player_sprite].collides_with(sprites[first_border_sprite + 0])) {
					  // When collides with the floor we can jump again
					  player_descending = false;
					  sprites[player_sprite].translate(0, +player_speed);
				  }
			  }
		  }
		  else {
			  // If the player is descending we continue going with negative speed
			  sprites[player_sprite].translate(0, -player_speed);
			  if (sprites[player_sprite].get_position().y() < -1.76f && sprites[player_sprite].get_position().y() >= -1.78f){
				  sprites[player_sprite].translate(0, -0.05f);
			  } 
			  if (sprites[player_sprite].collides_with(sprites[first_border_sprite + 0])) {
				  // When collides with the floor we can jump again
				  player_descending = false;
				  sprites[player_sprite].translate(0, +player_speed);
			  }
		  }
	  }
    }

	// We update the player position to make the jump effect
	void update_player_position() {
		// We check if the player is on the floor to avoid run innecessary code
		if (sprites[player_sprite].get_position().y() > -1.765f){
			// If the player position is not at height -1.8f the player has jumped so we push him down to the floor
			const float player_speed = 0.1f;
			sprites[player_sprite].translate(0, -player_speed);
			if (sprites[player_sprite].get_position().y() < -1.76f && sprites[player_sprite].get_position().y() >= -1.78f){
				sprites[player_sprite].translate(0, -0.05f);
			} 
			if (sprites[player_sprite].collides_with(sprites[first_border_sprite + 0])) {
				sprites[player_sprite].translate(0, +player_speed);
			}
		}
	}

	// We update the player position to make the jump effect
	void update_coin_position() {
		// Call to the method that rotate the sprite of the coin in the y axis the number of degrees as parameter
		sprites[coin_sprite].rotate_sprite_y(5);
	}

    // Check if the player collides with the left or top border
    bool player_collide(sprite &border) {
      sprite &playerer = sprites[first_border_sprite + (player_velocity < 0 ? 2 : 3)];
	  if (playerer.is_enabled() && playerer.collides_with(border)) {
        return true;
      }
      return false;
    }


    void draw_text(texture_shader &shader, float x, float y, float scale, const char *text) {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 1);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      enum { max_quads = 32 };
      bitmap_font::vertex vertices[max_quads*4];
      uint32_t indices[max_quads*6];
      aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

      unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font_texture);

      shader.render(modelToProjection, 0);

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x );
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u );
      glEnableVertexAttribArray(attribute_uv);

      glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
    }

  public:

    // this is called when we construct the class
    invaderers_app(int argc, char **argv) : app(argc, argv), font(512, 256, "assets/big.fnt") {
    }

    // this is called once OpenGL is initialized
    void app_init() {
      // set up the shader
      texture_shader_.init();

      // set up the matrices with a camera 5 units from the origin
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 0, 3);

      font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

      GLuint GameOver = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
      sprites[game_over_sprite].init(GameOver, 20, 0, 3, 1.5f);

	  // We use two background textures to make the sensation of never ending screen
	  GLuint background1 = resource_dict::get_texture_handle(GL_RGBA, "assets/assignment/Background.gif");
	  sprites[background_sprite + 0].init(background1, 0, 0, 6.0f, 6.0f);
	  GLuint background2 = resource_dict::get_texture_handle(GL_RGBA, "assets/assignment/Background.gif");
	  sprites[background_sprite + 1].init(background2, 6, 0, 6.0f, 6.0f);

	  // We use the coint texture
	  GLuint coin = resource_dict::get_texture_handle(GL_RGBA, "assets/assignment/coin2.gif");
	  sprites[coin_sprite].init(coin, 0, 0, 0.25f, 0.4f);

	  // We use the player texture
	  GLuint player = resource_dict::get_texture_handle(GL_RGBA, "assets/assignment/img.gif");
	  sprites[player_sprite].init(player, -2.75, -1.72, 0.505f, 0.70f);

	  // We set the border as sprite to walls collision
	  GLuint white = resource_dict::get_texture_handle(GL_RGB, "#ffffff");
	  sprites[first_border_sprite + 0].init(white, 0, -3, 6, 1.8f);
	  sprites[first_border_sprite + 1].init(white, 0, 3, 6, 0);
	  sprites[first_border_sprite + 2].init(white, -3, 0, 0, 6);
	  sprites[first_border_sprite + 3].init(white, 3, 0, 0, 6);

      // sundry counters and game state.
      num_lives = 3;
      game_over = false;
      score = 0;
    }

    // called every frame to move things
    void simulate() {
      if (game_over) {
        return;
      }

	  move_player();

	  // In case of the player jumping that brings him to the floor
	  update_player_position();
	  // Each frame we rotate the coin
	  update_coin_position();

	  // Border collision modify to check when the person is bouncing with the walls
	  sprite &border = sprites[player_sprite];
	  if (player_collide(border)) {
		player_velocity = -player_velocity;
      }
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      simulate();

      // set a viewport - includes whole window area
      glViewport(x, y, w, h);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // draw all the sprites
      for (int i = 0; i != num_sprites; ++i) {
        sprites[i].render(texture_shader_, cameraToWorld);
      }

      char score_text[32];
      sprintf(score_text, "score: %d   lives: %d\n", score, num_lives);
	  //printf("CAM X: %f \n", cameraToWorld.colx().length());
	  // We calculate the new position of the score
	  position_score = -1.5f + cameraToWorld.row(3).x();
	  //printf("POS SCORE : %f \n", position_score);
	  draw_text(texture_shader_, position_score, 2, 1.0f / 256, score_text);

      // move the listener with the camera
      vec4 &cpos = cameraToWorld.w();
      alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());
    }
  };
}
