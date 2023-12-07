struct LandmarkXY
{
    float x; // to calculate subsequent delta_x
    float y; // to calculate subsequent delta_y
    LandmarkXY(float lx, float ly)
    {
        x = lx;
        y = ly;
    }
};