static const int cos_table_100[] = {
    100, 96, 86, 70, 50, 25, 0, -25, -50, -70, -86, -96,
    -100, -96, -86, -70, -50, -25, 0, 25, 50, 70, 86, 96
};
static const int sin_table_100[] = {
    0, 25, 50, 70, 86, 96, 100, 96, 86, 70, 50, 25,
    0, -25, -50, -70, -86, -96, -100, -96, -86, -70, -50, -25
};
void get_circle_coords(int angle_deg, int* out_x, int* out_y) {
    if (angle_deg < 0) angle_deg += 360;
    angle_deg %= 360;
    int index = angle_deg / 15;
    *out_x = cos_table_100[index];
    *out_y = sin_table_100[index];
}
int absolute(int v) { return v < 0 ? -v : v; }
