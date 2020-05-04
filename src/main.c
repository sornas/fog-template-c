#include <stdio.h>

#define FOG_IMPL
#include "fog.h"

#include "main.h"

struct Player {
    Vec2 position;

    f32 speed;
};

struct Player player;

AssetID sound_id;
b8 playing = 0;

void new_sound() {
    AudioID id = fog_mixer_play_sound(0, sound_id, 1.0,
            AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 0);
    fog_mixer_post_sound_hook(id, new_sound);
}

void update() {
    f32 delta = fog_logic_delta();

    if (fog_input_down(NAME(UP), P1))
        player.position = fog_add_v2(player.position, fog_V2(0, +player.speed*delta));
    if (fog_input_down(NAME(DOWN), P1))
        player.position = fog_add_v2(player.position, fog_V2(0, -player.speed*delta));
    if (fog_input_down(NAME(LEFT), P1))
        player.position = fog_add_v2(player.position, fog_V2(-player.speed*delta, 0));
    if (fog_input_down(NAME(RIGHT), P1))
        player.position = fog_add_v2(player.position, fog_V2(+player.speed*delta, 0));

    if (fog_input_pressed(NAME(PLAY), P1)) {
        new_sound();
    }
}

void draw() {
    fog_renderer_push_point(1, player.position, fog_V4(1, 0, 0, 1), 0.1);
}

int main(int argc, char **argv) {
    for (u32 i = 0; i < NUM_BINDINGS; i++) {
        bindings[i] = fog_input_request_name(1);
    }

    fog_init(argc, argv);
    fog_renderer_set_window_size(WIN_WIDTH, WIN_HEIGHT);
    fog_renderer_turn_on_camera(0);

    fog_input_add(fog_key_to_input_code(SDLK_w), NAME(UP), P1);
    fog_input_add(fog_key_to_input_code(SDLK_s), NAME(DOWN), P1);
    fog_input_add(fog_key_to_input_code(SDLK_a), NAME(LEFT), P1);
    fog_input_add(fog_key_to_input_code(SDLK_d), NAME(RIGHT), P1);

    fog_input_add(fog_key_to_input_code(SDLK_SPACE), NAME(PLAY), P1);

    sound_id = fog_asset_fetch_id("NOISE_SHORT");

    fog_mixer_channel_set_delay(0, 0.3, 0.2, 0);

    player = (struct Player) {
        fog_V2(0, 0),
        2.0f,
    };

    fog_run(update, draw);
    return 0;
}
