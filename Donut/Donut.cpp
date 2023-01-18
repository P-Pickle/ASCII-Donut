#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>

char** CreateOutput(float screenwidth, float screenheight)
{
	char** output = new char*[screenwidth];

	for (int x = 0; x < screenwidth; x++)
	{
		output[x] = new char[screenheight];
	}

	for (int y = 0; y < screenheight; y++)
	{
		for (int x = 0; x < screenwidth; x++)
		{
			output[x][y] = ' ';
		}
	}

	return output;

}

void ResetOutput(char** output, float screenwidth, float screenheight)
{
	for (int y = 0; y < screenheight; y++)
	{
		for (int x = 0; x < screenwidth; x++)
		{
			output[x][y] = ' ';
		}
	}
}

float** CreateBuffer(float screenwidth, float screenheight)
{
	float** Zbuffer = new float*[screenwidth];

	for (int x = 0; x < screenwidth; x++)
	{
		Zbuffer[x] = new float[screenheight]();
	}

	
	return Zbuffer;
}

void ResetBuffer(float** zbuffer, float screenwidth, float screenheight)
{

	for (int y = 0; y < screenheight; y++)
	{
		for (int x = 0; x < screenwidth; x++)
		{
			zbuffer[x][y] = 0;
		}
	}
}


void Donut(float A, float B)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };

	const float screenwidth = 60;
	const float screenheight = 25;
	const float theta_spacing = 0.07;
	const float phi_spacing = .02;

	const float R1 = 1;
	const float R2 = 2;
	const float K2 = 5;

	const float K1 = (screenwidth * K2 * 3 / (8 * (R1 + R2)))/2;

	char** output = CreateOutput(screenwidth, screenheight);
	float** zbuffer = CreateBuffer(screenwidth, screenheight);
	

	while (true)//this is the loop that renders all the frames
	{
		ResetOutput(output, screenwidth, screenheight);
		ResetBuffer(zbuffer, screenwidth, screenheight);

		float CosA = cos(A), SinA = sin(A);
		float CosB = cos(B), SinB = sin(B);

		for (float theta = 0; theta < 2 * M_PI; theta += theta_spacing)
		{
			float CosTheta = cos(theta), SinTheta = sin(theta);

			for (float phi = 0; phi < 2 * M_PI; phi += phi_spacing)
			{
				float CosPhi = cos(phi), SinPhi = sin(phi);

				float circlex = R2 + R1 * CosTheta;
				float circley = R1 * SinTheta;

				float x = circlex * (CosB * CosPhi + SinA * SinB * SinPhi) - circley * CosA * SinB;
				float y = circlex * (SinB * CosPhi - SinA * CosB * SinPhi) + circley * CosA * CosB;
				float z = K2 + CosA * circlex * SinPhi + circley * SinA;

				float InvZ = 1 / z;

				int xp = (int)(screenwidth / 2 + K1 * InvZ * x);
				int yp = (int)(screenheight / 2 - K1 * InvZ * y);

				float L = CosPhi * CosTheta * SinB - CosA * CosTheta * SinPhi - SinA * SinTheta + CosB * (CosA * SinTheta - CosTheta * SinA * SinPhi);

				if (L > 0)
				{
					if (InvZ > zbuffer[xp][yp])
					{
						zbuffer[xp][yp] = InvZ;
						int Li = L * 8;

						output[xp][yp] = ".,-~:;=!*#$@"[Li];
					}
				}


			}


		}

		SetConsoleCursorPosition(hConsole, pos);
		for (int j = 0; j < screenheight; j++) {
			for (int i = 0; i < screenwidth; i++) {
				putchar(output[i] [j]);
			}
			putchar('\n');
		}

		A += 0.02;
		B += 0.02;
	}




}


void main()
{
	float A = 0, B = 0;


	Donut(A, B);


	return;
}