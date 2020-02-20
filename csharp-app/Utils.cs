using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Drawing2D;
using System.Drawing;


namespace MusicBallProLogs
{
    class Utils
    {
        public static int SHIFT = 420;
        static public  double get_radius_coor(int x, int y)
        {
            return Math.Sqrt((x * x) + (y * y));

        }


        static public double get_theta_coor(int x, int y)
        {

            return Math.Atan2(y, x);
        }

        static public double get_x_coor(double r, float theta)
        {

            return (r * Math.Cos(get_radian(-theta)));
        }


        static public double get_y_coor(double r, float theta)
        {

            return SHIFT + (r * Math.Sin(get_radian(-theta)));
        }

        public static float get_radian(float theta)
        {
            return theta * (float)Math.PI / 180F;
        }


        public static float get_degrees(float theta)
        {

            return (180F * theta) / (float)Math.PI;
        }

        static public void fill_circle(Graphics g, Brush brush,
                                     float centerX, float centerY, float radius)
        {
            g.FillEllipse(brush, centerX - radius, centerY - radius,
                          radius + radius, radius + radius);
        }
    }
}




