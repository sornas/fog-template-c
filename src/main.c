#include <stddef.h>

#define FOG_IMPL
#include "fog.h"

#include "main.h"

struct Player {
    Vec2 position;

    f32 speed;
};

struct Player player;

f32 runtime = 0;
s32 seconds = 0;

LogicID inc_runtime_id;
LogicID inc_seconds_id;

void inc_runtime(f32 timestep, f32 delta, f32 percentage, void *aux) {
    runtime += timestep;
}

void inc_seconds(f32 timestep, f32 delta, f32 percenage, void *aux) {
    seconds += 1;
}

void update() {
    f32 delta = fog_logic_delta();

    fog_util_show_f32("runtime", runtime);
    fog_util_show_s32("seconds", seconds);

    if (fog_input_down(NAME(UP), P1))
        player.position = fog_add_v2(player.position, fog_V2(0, +player.speed*delta));
    if (fog_input_down(NAME(DOWN), P1))
        player.position = fog_add_v2(player.position, fog_V2(0, -player.speed*delta));
    if (fog_input_down(NAME(LEFT), P1))
        player.position = fog_add_v2(player.position, fog_V2(-player.speed*delta, 0));
    if (fog_input_down(NAME(RIGHT), P1))
        player.position = fog_add_v2(player.position, fog_V2(+player.speed*delta, 0));
    if (fog_input_down(NAME(STOP), P1)) {
        fog_logic_remove_callback(inc_runtime_id);
        fog_logic_remove_callback(inc_seconds_id);
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

    inc_runtime_id = fog_logic_add_callback(
            PRE_UPDATE,   // when in the loop
            inc_runtime,  // what to do
            0,            // when to start (immediately)
            FOREVER,      // when to stop
            0.0,          // spacing       (none)
            NULL
    );

    inc_seconds_id = fog_logic_add_callback(
            PRE_UPDATE,   // when in the loop
            inc_seconds,  // what to do
            0,            // when to start (immediately)
            FOREVER,      // when to stop
            1.0,          // spacing       (1 second)
            NULL
    );

    fog_input_add(fog_key_to_input_code(SDLK_w), NAME(UP), P1);
    fog_input_add(fog_key_to_input_code(SDLK_s), NAME(DOWN), P1);
    fog_input_add(fog_key_to_input_code(SDLK_a), NAME(LEFT), P1);
    fog_input_add(fog_key_to_input_code(SDLK_d), NAME(RIGHT), P1);
    fog_input_add(fog_key_to_input_code(SDLK_SPACE), NAME(STOP), P1);

    player = (struct Player) {
        fog_V2(0, 0),
        2.0f,
    };

    fog_run(update, draw);
    return 0;
}
