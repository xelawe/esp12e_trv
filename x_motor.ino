const short PWM_R = D1;
const short DIR_R = D3;

const short PWM_L = D2;
const short DIR_L = D4;

int gv_speed = 1024;

#define vent_close HIGH
#define vent_open LOW

boolean gv_motor_run = false;
boolean gv_motor_dir = false;

int gv_act_pos;
int gv_dest_pos;
int gv_old_pos;
