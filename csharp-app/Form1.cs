using System;
using System.Threading;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.Reflection;

namespace MusicBallProLogs
{
	public partial class Form1 : Form
	{
                static Stopwatch stopwatch;  
		static private DataTable table;
		private static int[,] buffer = new int[9, 4];
		private static int[,] RANGE_REF_VALUES = new int[4, 5]{ 
			{9700, 12000, 18000, 13000, 10000}, 
				{8000, 8600, 9700, 7900, 8600}, 
				{5800, 6700, 6000, 5600, 6600}, 
				{2600, 4000, 3700, 4000, 3400}, 
		};
		private static int[] ANGLE_REF_VALUES = new int[5]{44, 36, 45, 54, 72}; 

		private PortChat serialPort;

		private const int UNIT_ANGLE = 18;
		private const int NB_ANGLES = 90 / UNIT_ANGLE; 
		private int debugCounter = 0; 
		private Boolean isMouseUp = true; 
		private int radius_index  = 0;  
		private int radius_index_old  = 0;  
		private int theta_index = 0;  
		private int theta_index_old = 0;  



		static private Boolean _continue_display = true; 

		// calculation vars 

		static double PI = 3.14159265;
		static float radian = (float)PI / 180;


		Int16[] angle = { 15, 45, 75}; 
		Int16[] emitter = { 18, 36, 54, 72}; 

		static float[] ex = new float[10];
		static float[] ey = new float[10];
		static float[] rx = new float[10];
		static float[] ry = new float[10];    
		private Thread displayThread;  


		System.Drawing.Graphics graphicsObj; 

		protected override CreateParams CreateParams {
			get {
				CreateParams cp = base.CreateParams;
				cp.ExStyle |= 0x02000000;  // Turn on WS_EX_COMPOSITED
				return cp;
			}
		} 


		public Form1()
		{
			InitializeComponent();
			/*     this.SetStyle((System.Windows.Forms.ControlStyles.UserPaint |
			       System.Windows.Forms.ControlStyles.AllPaintingInWmPaint |
			       System.Windows.Forms.ControlStyles.OptimizedDoubleBuffer),
			       true);
			       */    
			InitGridView();
			InitCalc();
			this.SetStyle(ControlStyles.DoubleBuffer, true);
			this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
			this.SetStyle(ControlStyles.UserPaint, true);
			this.SetStyle(ControlStyles.SupportsTransparentBackColor, false);
			this.SetStyle(ControlStyles.Opaque, false);
			this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
			this.SetStyle(ControlStyles.ResizeRedraw, true);


			displayThread = new Thread( DisplayCoordinates );
			displayThread.Start(); 

			stopwatch = new Stopwatch();
			stopwatch.Reset();
			stopwatch.Start();


		}


		private void InitGridView() {
			// dataGridView1.SelectionMode = DataGridViewSelectionMode.;
			table = new DataTable();
			table.Columns.Add("S01", typeof(int));
			table.Columns.Add("S02", typeof(int));
			table.Columns.Add("S03", typeof(int));

			for (int i = 0; i < 4; i++)
			{
				table.Rows.Add(0, 0, 0);   
			}


			dataGridView1.DataSource = table;


			serialPort = new PortChat(table, "com5"); 

		}





		private void label1_Click(object sender, EventArgs e)
		{

		}

		private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
		{

		}




		void InitCalc() {

			for (int i = 0; i < 4; i++)
			{
				ex[i] = (float)Math.Sin(radian * emitter[i]);
				ey[i] = (float)Math.Cos(radian * emitter[i]);
			}

			for (int i = 0; i < 3; i++)
			{
				rx[i] = (float)Math.Sin(radian * angle[i]);
				ry[i] = (float)Math.Cos(radian * angle[i]);
			}                                                                

		}
              
		public static void calc_coordinates( out float out_phi, out float out_range) {
		
				float sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
                                int nonEmpty = 0; 

                         
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						try
						{
							/*       
								 sum1 += ex[j] * float.Parse(table.Rows[j][i].ToString());  
								 sum2 += ey[j] * float.Parse(table.Rows[j][i].ToString());
								 sum3 += rx[i] * float.Parse(table.Rows[j][i].ToString()); 
								 sum4 += ry[i] * float.Parse(table.Rows[j][i].ToString());
								 */      
							sum1 += ex[j] * PortChat.buffer[i, j];  
							sum2 += ey[j] * PortChat.buffer[i, j];  
							sum3 += rx[i] * PortChat.buffer[i, j];
							sum4 += ry[i] * PortChat.buffer[i, j];

							if(PortChat.buffer[i, j] != 0) nonEmpty++; 

						}
						catch (IndexOutOfRangeException e)
						{
							Debug.WriteLine(" ####################  --------------ex: " + e.Message + " " + i + " " + j);
						}
						catch (FormatException e)
						{
							Debug.WriteLine(" ex: " + e.Message);
						}

					}
					// Console.WriteLine();
				}                                                         
                               
				if(nonEmpty == 0) nonEmpty = 1; 

				float Ex = sum1, Ey = sum2, Rx = sum3, Ry = sum4;                         
				float phi = (float) Math.Atan2( Ex, Ey);                                               
				float range = ((float) Math.Sqrt((Rx * Rx) + (Ry * Ry)));
				//float range = ((float) Math.Sqrt((Rx * Rx) + (Ry * Ry)))/ nonEmpty;


				phi = phi / radian;
				out_phi = phi;
				out_range = range; 


		
		}

		public static void calc_index(float phi, float range, out int out_angle_index, out int out_range_index) {
		
		                int angle_index = 0; 
				for(int i=0; i < 5; i++) {
					if (Math.Abs(ANGLE_REF_VALUES[i] - phi) < 5) {
						angle_index = i;  
					} 	
				}

				int range_index = 0; 
				for(int i=0; i < 4; i++) { 
					if (Math.Abs(RANGE_REF_VALUES[i, angle_index] - range) < 1000) { 
						range_index = i;  
					}
				}
	
				out_angle_index = angle_index; 
				out_range_index = range_index; 
		
		
		}
               
		public void DisplayCoordinates() { 

			while( _continue_display) {

				//    SetTextBoxText(textPosAngle, range.ToString("0"));
				float phi = 0, range = 0; 
				calc_coordinates(out phi, out range); 

				SetTextBoxText(textPosAngle, phi.ToString("0"));
				SetTextBoxText(textRange, range.ToString("0"));


				int angle_index = 0, range_index = 0; 
				calc_index(phi, range, out angle_index, out range_index);   
				theta_index = 4 - angle_index; 
				radius_index = range_index; 

				if(theta_index_old != theta_index   || radius_index_old != radius_index) 
				{
					panel5.Invalidate(); 
					isMouseUp = (  range != 0 || phi != 0 )? false : true;
				} 
				radius_index_old = radius_index; 
				theta_index_old = theta_index; 

				Thread.Sleep(5);                                                                

			}
		}


		static void SetTextBoxText(TextBox text_box, string text) {
			text_box.BeginInvoke(new MethodInvoker(() =>
						{
						text_box.Text = text;

						}));


		}

		private void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			_continue_display = false; 
			displayThread.Join(); 
			serialPort.Close(); 
		}


		private void panelSectorDrawing_Paint(object sender, PaintEventArgs e) {
			panel_drawing(sender, e);
			//   Debug.WriteLine("panel_drawing ---------"); 
		}
		private void panel_drawing(object sender, PaintEventArgs e)
		{
			System.Drawing.Graphics graphicsObj;

			graphicsObj = panel5.CreateGraphics();

			graphicsObj.SmoothingMode = SmoothingMode.AntiAlias; // lissage des graphiques
			Pen myPen = new Pen(System.Drawing.Color.Red, 2);

			LinearGradientBrush linGrBrush = new LinearGradientBrush(
					new Point(400, 10),
					new Point(0, 10),
					Color.FromArgb(160, 30, 80, 20),
					Color.FromArgb(160, 30, 90, 30));

			//     Pen pen = new Pen(linGrBrush);
			linGrBrush.GammaCorrection = true;


			SolidBrush brush = new SolidBrush(Color.Red);
			PointF[] pts = new PointF[3];

			float x1 = 0;
			float y1 = Utils.SHIFT;
			float x2 = 100;
			float y2 = Utils.SHIFT;

			pts[0] = new PointF(x1, y1);
			pts[2] = new PointF(x2, y2);
			Pen pen = new Pen(linGrBrush);



			int angle = 0;
			int radius = 100;


			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < NB_ANGLES; i++)
				{
					angle += 18;
					pts[1] = pts[2];
					pts[2].X = (float)Utils.get_x_coor(radius, angle);
					pts[2].Y = (float)Utils.get_y_coor(radius, angle);

					graphicsObj.DrawPolygon(pen, pts);
				}
				angle = 0;
				radius += 100;
				pts[1].X = radius;
				pts[1].Y = Utils.SHIFT;
				pts[2].X = (float)Utils.get_x_coor(radius, angle);
				pts[2].Y = (float)Utils.get_y_coor(radius, angle);


			}


			if (!isMouseUp)
			{
				PointF[] pts4 = new PointF[4];

				pts4[0] = new PointF((float)Utils.get_x_coor(radius_index * 100, theta_index * UNIT_ANGLE),
						(float)Utils.get_y_coor(radius_index * 100, theta_index * UNIT_ANGLE));
				pts4[2] = new PointF((float)Utils.get_x_coor((radius_index + 1) * 100, (theta_index + 1) * UNIT_ANGLE),
						(float)Utils.get_y_coor((radius_index + 1) * 100, (theta_index + 1) * UNIT_ANGLE));

				pts4[1] = new PointF((float)Utils.get_x_coor((radius_index + 1) * 100, theta_index * UNIT_ANGLE),
						(float)Utils.get_y_coor((radius_index + 1) * 100, theta_index * UNIT_ANGLE));
				pts4[3] = new PointF((float)Utils.get_x_coor(radius_index * 100, (theta_index + 1) * UNIT_ANGLE),
						(float)Utils.get_y_coor(radius_index * 100, (theta_index + 1) * UNIT_ANGLE));

				graphicsObj.FillPolygon(new SolidBrush(Color.Blue), pts4);



			}

			// Debug.WriteLine("onPaint --  " + isMouseUp);



		}

		private void panel5_MouseUp(object sender, MouseEventArgs e)
		{

		}

		private void panel5_MouseDown(object sender, MouseEventArgs e)
		{
			float theta = Utils.get_degrees( (float)Utils.get_theta_coor(e.Location.X, (Utils.SHIFT - e.Location.Y))); 
			float radius = (float)Utils.get_radius_coor(e.Location.X, (Utils.SHIFT - e.Location.Y));
			theta_index =  (int)theta / UNIT_ANGLE;  
			radius_index =  (int)radius / 100 ;

			isMouseUp = !isMouseUp ; 
			//	Debug.WriteLine("-----------   " + isMouseUp);
			panel5.Invalidate(); 

		}

		private void dataGridView1_DataError(object sender, DataGridViewDataErrorEventArgs e)
		{
			Debug.WriteLine("  dataGridView DataError ---------------------" + e.Exception.Message);            
		}


		protected override void OnPaint(PaintEventArgs e) {

			// panel_drawing(this, e);
		}

	}



}
















