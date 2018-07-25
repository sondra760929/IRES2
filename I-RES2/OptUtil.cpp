#include "StdAfx.h"
#include "OptUtil.h"

static double signa[2] = {1.0, -1.0};

float COptUtil::_IGRIP_COLOR_INDEX[35][3] = {
	{0.0f, 1.0f, 1.0f},		//	1 White
	{0.0f, 1.0f, 1.0f},		//	2 White
	{0.0f, 1.0f, 1.0f},		//	3 White
	{1.0f, 1.0f, 1.0f},		//	4 White
	{0.5f, 0.5f, 0.5f},		//	5 Grey
	{0.3f, 0.3f, 0.3f},		//	6 Black
	{1.0f, 0.0f, 0.0f},		//	7 Red
	{0.0f, 1.0f, 0.0f},		//	8 Green
	{0.0f, 0.0f, 1.0f},		//	9 Blue
	{1.0f, 1.0f, 0.0f},		//	10 Yellow
	{0.0f, 1.0f, 1.0f},		//	11 Cyan
	{0.75f, 0.2f, 1.0f},	//	12 Purple
	{1.0f, 0.375f, 0.0f},	//	13 Orange
	{1.0f, 0.0f, 1.0f},		//	14 Magenta
	{1.0f, 0.2f, 0.5f},		//	15 Cherry
	{0.6f, 0.6f, 1.0f},		//	16 Light Blue
	{0.5f, 0.1875f, 0.0f},	//	17 Brown
	{0.0f, 0.5f, 0.0f},		//	18 Lime
	{1.0f, 0.6667f, 0.5f},	//	19 Tan
	{0.0f, 0.0f, 0.0f},		//	20 All Black
	{0.75f, 0.75f, 0.75f},	//	21 Grey-75
	{0.39f, 0.4f, 0.36f},	//	22 Slate Grey
	{0.0f, 0.5f, 0.5f},		//	23 Teal
	{0.0f, 0.3f, 0.55f},	//	24 Dodger Blue
	{0.1f, 0.1f, 0.34f},	//	25 Cobalt
	{0.25f, 0.1f, 0.25f},	//	26 Dark Plum
	{1.0f, 0.75f, 0.2f},	//	27 Brass
	{0.7f, 0.13f, 0.13f},	//	28 Fire Brick
	{1.0f, 0.6f, 0.05f},	//	29 Light Orange
	{0.37f, 0.57f, 0.62f},	//	30 Cadet Blue
	{0.47f, 0.58f, 0.52f},	//	31 Slate Green
	{0.2f, 0.66f, 0.33f},	//	32 Light Green
	{0.45f, 0.47f, 0.33f},	//	33 Army Green
	{0.6f, 0.5f, 0.4f},		//	34 Putty
	{0.85f, 0.72f, 0.55f}	//	35 Khaki
};
float COptUtil::_IGRIP_TO_ROBCAD_COLOR_INDEX [35] = {
	7, 7, 7, 7, 10,
	0, 1, 2, 4, 3,
	6, 17, 8, 5, 9,
	17, 12, 14, 9, 0,
	10, 10, 6, 6, 10,
	10, 13, 13, 8, 11,
	10, 10, 10, 10, 9
};

float COptUtil::_ROBCAD_COLOR_INDEX [18][3] = {
	{0.0f, 0.0f, 0.0f},			//	0 Black
	{1.0f, 0.0f, 0.0f},			//	1 Red
	{0.0f, 1.0f, 0.0f},			//	2 Green
	{1.0f, 1.0f, 0.0f},			//	3 Yellow
	{0.0f, 0.0f, 1.0f},			//	4 Blue
	{1.0f, 0.0f, 1.0f},			//	5 Magenta
	{0.0f, 1.0f, 1.0f},			//	6 Cyan
	{1.0f, 1.0f, 1.0f},			//	7 white
	{1.0f, 0.38f, 0.0f},		//	8 Orange
	{1.0f, 0.75f, 0.8f},		//	9 pink
	{0.5f, 0.5f, 0.5f},			//	10 Grey
	{0.9f, 0.9f, 1.0f},			//	11 blue_grey
	{0.5f, 0.19f, 0.0f},		//	12 Brown
	{0.5f, 0.37f, 0.26f},		//	13 wood
	{0.0f, 0.39f, 0.0f},		//	14 dark_green
	{0.55f, 0.0f, 0.0f},		//	15 dark_red
	{0.36f, 0.25f, 0.2f},		//	16 dark_brown
	{0.6f, 0.6f, 1.0f}			//	17 Light Blue
};

char* COptUtil::_ROBCAD_COLOR_NAME [18] = {
	"BLACK",
	"RED",
	"GREEN",
	"YELLOW",
	"BLUE",
	"MAGENTA",
	"CYAN",
	"WHITE",
	"ORANGE",
	"PINK",
	"GREY",
	"BLUE_GREY",
	"BROWN",
	"WOOD",
	"DARK_GREEN",
	"DARK_RED",
	"DARK_BROWN",
	"LIGHT_BLUE"
};

COptUtil::COptUtil(void)
{
	_IGRIP_COLOR_INDEX[0][0] = 0.0f;		_IGRIP_COLOR_INDEX[0][1] = 1.0f;		_IGRIP_COLOR_INDEX[0][2] = 1.0f;
	_IGRIP_COLOR_INDEX[1][0] = 0.0f;		_IGRIP_COLOR_INDEX[1][1] = 1.0f;		_IGRIP_COLOR_INDEX[1][2] = 1.0f;
	_IGRIP_COLOR_INDEX[2][0] = 0.0f;		_IGRIP_COLOR_INDEX[2][1] = 1.0f;		_IGRIP_COLOR_INDEX[2][2] = 1.0f;
	_IGRIP_COLOR_INDEX[3][0] = 1.0f;		_IGRIP_COLOR_INDEX[3][1] = 1.0f;		_IGRIP_COLOR_INDEX[3][2] = 1.0f;
	_IGRIP_COLOR_INDEX[4][0] = 0.5f;		_IGRIP_COLOR_INDEX[4][1] = 0.5f;		_IGRIP_COLOR_INDEX[4][2] = 0.5f;
	_IGRIP_COLOR_INDEX[5][0] = 0.3f;		_IGRIP_COLOR_INDEX[5][1] = 0.3f;		_IGRIP_COLOR_INDEX[5][2] = 0.3f;
	_IGRIP_COLOR_INDEX[6][0] = 1.0f;		_IGRIP_COLOR_INDEX[6][1] = 0.0f;		_IGRIP_COLOR_INDEX[6][2] = 0.0f;
	_IGRIP_COLOR_INDEX[7][0] = 0.0f;		_IGRIP_COLOR_INDEX[7][1] = 1.0f;		_IGRIP_COLOR_INDEX[7][2] = 0.0f;
	_IGRIP_COLOR_INDEX[8][0] = 0.0f;		_IGRIP_COLOR_INDEX[8][1] = 0.0f;		_IGRIP_COLOR_INDEX[8][2] = 1.0f;
	_IGRIP_COLOR_INDEX[9][0] = 1.0f;		_IGRIP_COLOR_INDEX[9][1] = 1.0f;		_IGRIP_COLOR_INDEX[9][2] = 0.0f;
	_IGRIP_COLOR_INDEX[10][0] = 0.0f;		_IGRIP_COLOR_INDEX[10][1] = 1.0f;		_IGRIP_COLOR_INDEX[10][2] = 1.0f;
	_IGRIP_COLOR_INDEX[11][0] = 0.75f;		_IGRIP_COLOR_INDEX[11][1] = 0.2f;		_IGRIP_COLOR_INDEX[11][2] = 1.0f;
	_IGRIP_COLOR_INDEX[12][0] = 1.0f;		_IGRIP_COLOR_INDEX[12][1] = 0.375f;		_IGRIP_COLOR_INDEX[12][2] = 0.0f;
	_IGRIP_COLOR_INDEX[13][0] = 1.0f;		_IGRIP_COLOR_INDEX[13][1] = 0.0f;		_IGRIP_COLOR_INDEX[13][2] = 1.0f;
	_IGRIP_COLOR_INDEX[14][0] = 1.0f;		_IGRIP_COLOR_INDEX[14][1] = 0.2f;		_IGRIP_COLOR_INDEX[14][2] = 0.5f;
	_IGRIP_COLOR_INDEX[15][0] = 0.6f;		_IGRIP_COLOR_INDEX[15][1] = 0.6f;		_IGRIP_COLOR_INDEX[15][2] = 1.0f;
	_IGRIP_COLOR_INDEX[16][0] = 0.5f;		_IGRIP_COLOR_INDEX[16][1] = 0.1875f;	_IGRIP_COLOR_INDEX[16][2] = 0.0f;
	_IGRIP_COLOR_INDEX[17][0] = 0.0f;		_IGRIP_COLOR_INDEX[17][1] = 0.5f;		_IGRIP_COLOR_INDEX[17][2] = 0.0f;
	_IGRIP_COLOR_INDEX[18][0] = 1.0f;		_IGRIP_COLOR_INDEX[18][1] = 0.6667f;	_IGRIP_COLOR_INDEX[18][2] = 0.5f;
	_IGRIP_COLOR_INDEX[19][0] = 0.0f;		_IGRIP_COLOR_INDEX[19][1] = 0.0f;		_IGRIP_COLOR_INDEX[19][2] = 0.0f;
	_IGRIP_COLOR_INDEX[20][0] = 0.75f;		_IGRIP_COLOR_INDEX[20][1] = 0.75f;		_IGRIP_COLOR_INDEX[20][2] = 0.75f;
	_IGRIP_COLOR_INDEX[21][0] = 0.39f;		_IGRIP_COLOR_INDEX[21][1] = 0.4f;		_IGRIP_COLOR_INDEX[21][2] = 0.36f;
	_IGRIP_COLOR_INDEX[22][0] = 0.0f;		_IGRIP_COLOR_INDEX[22][1] = 0.5f;		_IGRIP_COLOR_INDEX[22][2] = 0.5f;
	_IGRIP_COLOR_INDEX[23][0] = 0.0f;		_IGRIP_COLOR_INDEX[23][1] = 0.3f;		_IGRIP_COLOR_INDEX[23][2] = 0.55f;
	_IGRIP_COLOR_INDEX[24][0] = 0.1f;		_IGRIP_COLOR_INDEX[24][1] = 0.1f;		_IGRIP_COLOR_INDEX[24][2] = 0.34f;
	_IGRIP_COLOR_INDEX[25][0] = 0.25f;		_IGRIP_COLOR_INDEX[25][1] = 0.1f;		_IGRIP_COLOR_INDEX[25][2] = 0.25f;
	_IGRIP_COLOR_INDEX[26][0] = 1.0f;		_IGRIP_COLOR_INDEX[26][1] = 0.75f;		_IGRIP_COLOR_INDEX[26][2] = 0.2f;
	_IGRIP_COLOR_INDEX[27][0] = 0.7f;		_IGRIP_COLOR_INDEX[27][1] = 0.13f;		_IGRIP_COLOR_INDEX[27][2] = 0.13f;
	_IGRIP_COLOR_INDEX[28][0] = 1.0f;		_IGRIP_COLOR_INDEX[28][1] = 0.6f;		_IGRIP_COLOR_INDEX[28][2] = 0.05f;
	_IGRIP_COLOR_INDEX[29][0] = 0.37f;		_IGRIP_COLOR_INDEX[29][1] = 0.57f;		_IGRIP_COLOR_INDEX[29][2] = 0.62f;
	_IGRIP_COLOR_INDEX[30][0] = 0.47f;		_IGRIP_COLOR_INDEX[30][1] = 0.58f;		_IGRIP_COLOR_INDEX[30][2] = 0.52f;
	_IGRIP_COLOR_INDEX[31][0] = 0.2f;		_IGRIP_COLOR_INDEX[31][1] = 0.66f;		_IGRIP_COLOR_INDEX[31][2] = 0.33f;
	_IGRIP_COLOR_INDEX[32][0] = 0.45f;		_IGRIP_COLOR_INDEX[32][1] = 0.47f;		_IGRIP_COLOR_INDEX[32][2] = 0.33f;
	_IGRIP_COLOR_INDEX[33][0] = 0.6f;		_IGRIP_COLOR_INDEX[33][1] = 0.5f;		_IGRIP_COLOR_INDEX[33][2] = 0.4f;
	_IGRIP_COLOR_INDEX[34][0] = 0.85f;		_IGRIP_COLOR_INDEX[34][1] = 0.72f;		_IGRIP_COLOR_INDEX[34][2] = 0.55f;

	_IGRIP_TO_ROBCAD_COLOR_INDEX[0] = 7;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[1] = 7;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[2] = 7;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[3] = 7;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[4] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[5] = 0;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[6] = 1;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[7] = 2;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[8] = 4;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[9] = 3;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[10] = 6;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[11] = 17;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[12] = 8;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[13] = 5;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[14] = 9;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[15] = 17;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[16] = 12;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[17] = 14;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[18] = 9;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[19] = 0;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[20] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[21] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[22] = 6;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[23] = 6;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[24] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[25] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[26] = 13;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[27] = 13;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[28] = 8;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[29] = 11;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[30] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[31] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[32] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[33] = 10;
	_IGRIP_TO_ROBCAD_COLOR_INDEX[34] = 9;

	_ROBCAD_COLOR_INDEX [0][0] = 0.0f;		_ROBCAD_COLOR_INDEX [0][1] = 0.0f;		_ROBCAD_COLOR_INDEX [0][2] = 0.0f;			//	0 Black
	_ROBCAD_COLOR_INDEX [1][0] = 1.0f;		_ROBCAD_COLOR_INDEX [1][1] = 0.0f;		_ROBCAD_COLOR_INDEX [1][2] = 0.0f;			//	1 Red
	_ROBCAD_COLOR_INDEX [2][0] = 0.0f;		_ROBCAD_COLOR_INDEX [2][1] = 1.0f;		_ROBCAD_COLOR_INDEX [2][2] = 0.0f;			//	2 Green
	_ROBCAD_COLOR_INDEX [3][0] = 1.0f;		_ROBCAD_COLOR_INDEX [3][1] = 1.0f;		_ROBCAD_COLOR_INDEX [3][2] = 0.0f;			//	3 Yellow
	_ROBCAD_COLOR_INDEX [4][0] = 0.0f;		_ROBCAD_COLOR_INDEX [4][1] = 0.0f;		_ROBCAD_COLOR_INDEX [4][2] = 1.0f;			//	4 Blue
	_ROBCAD_COLOR_INDEX [5][0] = 1.0f;		_ROBCAD_COLOR_INDEX [5][1] = 0.0f;		_ROBCAD_COLOR_INDEX [5][2] = 1.0f;			//	5 Magenta
	_ROBCAD_COLOR_INDEX [6][0] = 0.0f;		_ROBCAD_COLOR_INDEX [6][1] = 1.0f;		_ROBCAD_COLOR_INDEX [6][2] = 1.0f;			//	6 Cyan
	_ROBCAD_COLOR_INDEX [7][0] = 1.0f;		_ROBCAD_COLOR_INDEX [7][1] = 1.0f;		_ROBCAD_COLOR_INDEX [7][2] = 1.0f;			//	7 white
	_ROBCAD_COLOR_INDEX [8][0] = 1.0f;		_ROBCAD_COLOR_INDEX [8][1] = 0.38f;		_ROBCAD_COLOR_INDEX [8][2] = 0.0f;		//	8 Orange
	_ROBCAD_COLOR_INDEX [9][0] = 1.0f;		_ROBCAD_COLOR_INDEX [9][1] = 0.75f;		_ROBCAD_COLOR_INDEX [9][2] = 0.8f;		//	9 pink
	_ROBCAD_COLOR_INDEX [10][0] = 0.5f;		_ROBCAD_COLOR_INDEX [10][1] = 0.5f;		_ROBCAD_COLOR_INDEX [10][2] = 0.5f;			//	10 Grey
	_ROBCAD_COLOR_INDEX [11][0] = 0.9f;		_ROBCAD_COLOR_INDEX [11][1] = 0.9f;		_ROBCAD_COLOR_INDEX [11][2] = 1.0f;			//	11 blue_grey
	_ROBCAD_COLOR_INDEX [12][0] = 0.5f;		_ROBCAD_COLOR_INDEX [12][1] = 0.19f;	_ROBCAD_COLOR_INDEX [12][2] = 0.0f;		//	12 Brown
	_ROBCAD_COLOR_INDEX [13][0] = 0.5f;		_ROBCAD_COLOR_INDEX [13][1] = 0.37f;	_ROBCAD_COLOR_INDEX [13][2] = 0.26f;		//	13 wood
	_ROBCAD_COLOR_INDEX [14][0] = 0.0f;		_ROBCAD_COLOR_INDEX [14][1] = 0.39f;	_ROBCAD_COLOR_INDEX [14][2] = 0.0f;		//	14 dark_green
	_ROBCAD_COLOR_INDEX [15][0] = 0.55f;	_ROBCAD_COLOR_INDEX [15][1] = 0.0f;		_ROBCAD_COLOR_INDEX [15][2] = 0.0f;		//	15 dark_red
	_ROBCAD_COLOR_INDEX [16][0] = 0.36f;	_ROBCAD_COLOR_INDEX [16][1] = 0.25f;	_ROBCAD_COLOR_INDEX [16][2] = 0.2f;		//	16 dark_brown
	_ROBCAD_COLOR_INDEX [17][0] = 0.6f;		_ROBCAD_COLOR_INDEX [17][1] = 0.6f;		_ROBCAD_COLOR_INDEX [17][2] = 1.0f;		//	17 Light Blue

	_ROBCAD_COLOR_NAME [0] = "BLACK";
	_ROBCAD_COLOR_NAME [1] = "RED";
	_ROBCAD_COLOR_NAME [2] = "GREEN";
	_ROBCAD_COLOR_NAME [3] = "YELLOW";
	_ROBCAD_COLOR_NAME [4] = "BLUE";
	_ROBCAD_COLOR_NAME [5] = "MAGENTA";
	_ROBCAD_COLOR_NAME [6] = "CYAN";
	_ROBCAD_COLOR_NAME [7] = "WHITE";
	_ROBCAD_COLOR_NAME [8] = "ORANGE";
	_ROBCAD_COLOR_NAME [9] = "PINK";
	_ROBCAD_COLOR_NAME [10] = "GREY";
	_ROBCAD_COLOR_NAME [11] = "BLUE_GREY";
	_ROBCAD_COLOR_NAME [12] = "BROWN";
	_ROBCAD_COLOR_NAME [13] = "WOOD";
	_ROBCAD_COLOR_NAME [14] = "DARK_GREEN";
	_ROBCAD_COLOR_NAME [15] = "DARK_RED";
	_ROBCAD_COLOR_NAME [16] = "DARK_BROWN";
	_ROBCAD_COLOR_NAME [17] = "LIGHT_BLUE";
}

COptUtil::~COptUtil(void)
{
}

int COptUtil::GetRobcadColorIndex(CString color_name)
{
	for (int i=0; i<18; i++)
	{
		if (_ROBCAD_COLOR_NAME[i] == color_name)
		{
			return i;
		}
	}
	return -1;
}
int COptUtil::GetIgripColorIndex(COLORREF color_ref)
{
	float fr = (float)GetRValue(color_ref) / 255.0f;
	float fg = (float)GetGValue(color_ref) / 255.0f;
	float fb = (float)GetBValue(color_ref) / 255.0f;
	int current_index = -1;
	float max_offset(1.0f);
	for (int i=0; i<35; i++)
	{
		float fr_offset = fabs(_IGRIP_COLOR_INDEX[i][0] - fr);
		float fg_offset = fabs(_IGRIP_COLOR_INDEX[i][1] - fg);
		float fb_offset = fabs(_IGRIP_COLOR_INDEX[i][2] - fb);
		float current_offset = max(fr_offset, max(fg_offset, fb_offset));
		if (current_offset < max_offset)
		{
			max_offset = current_offset;
			current_index = i;
		}
	}
	return current_index + 1;
}

int COptUtil::SplitString(CString& str_input, vector< char >& split_chrs_input, vector< CString >& str_output)
{
	str_output.clear();
	vector< int > split_index_array;
	int split_count = (int)split_chrs_input.size();
	int temp_split_index;

	for(int split_index=0; split_index<split_count; ++split_index)
	{
		temp_split_index = str_input.Find(split_chrs_input[split_index]);
		while(temp_split_index > -1)
		{
			split_index_array.push_back(temp_split_index);
			temp_split_index = str_input.Find(split_chrs_input[split_index], temp_split_index+1);
		}
	}

	sort(split_index_array.begin(), split_index_array.end());
	split_count = (int)split_index_array.size();
	temp_split_index = -1;
	CString temp_split_str;
	for(int split_index=0; split_index<split_count; ++split_index)
	{
		if(temp_split_index < split_index_array[split_index])
		{
			temp_split_str = str_input.Mid(temp_split_index+1, split_index_array[split_index]-temp_split_index-1);
			temp_split_index = split_index_array[split_index];
			if(temp_split_str != "")
			{
				str_output.push_back(temp_split_str);
			}
		}
	}

	if(temp_split_index < str_input.GetLength())
	{
		temp_split_str = str_input.Mid(temp_split_index+1, str_input.GetLength()-1-temp_split_index);
		if(temp_split_str != "")
		{
			str_output.push_back(temp_split_str);
		}
	}

	return (int)str_output.size();
}

int COptUtil::SplitString(char* str_input, vector< char >& split_chrs_input, vector< CString >& str_output)
{
	str_output.clear();
	int char_index(0);
	char temp_string[512];
	int temp_char_index(0);
	bool b_splitted(false);
	int split_count = (int)split_chrs_input.size();
	while(str_input[char_index] != '\0')
	{
		b_splitted = false;
		for(int split_index=0; split_index<split_count; ++split_index)
		{
			if(split_chrs_input[split_index] == str_input[char_index])
			{
				b_splitted = true;
				if(temp_char_index > 0)
				{
					temp_string[temp_char_index] = '\0';
					CString temp_str_output(temp_string);
					temp_str_output.Trim();
					str_output.push_back(temp_str_output);
				}
				temp_char_index = 0;
				char_index++;
				break;
			}
		}

		if(b_splitted == false)
		{
			temp_string[temp_char_index++] = str_input[char_index++];
		}
	}

	if(temp_char_index > 0)
	{
		temp_string[temp_char_index] = '\0';
		CString temp_str_output(temp_string);
		temp_str_output.Trim();
		if(temp_str_output != "")
		{
			str_output.push_back(temp_str_output);
		}
	}
	return (int)str_output.size();
}

int COptUtil::SplitString(string& str_input, vector< char >& split_chrs_input, vector< string >& str_output)
{
	str_output.clear();
	vector< int > split_index_array;
	CString str_input_cstring;
	str_input_cstring.Format(TEXT("%s"), str_input.c_str());
	int split_count = (int)split_chrs_input.size();
	int temp_split_index;

	for(int split_index=0; split_index<split_count; ++split_index)
	{
		temp_split_index = str_input.find_first_of(split_chrs_input[split_index]);
		while(temp_split_index != string::npos)
		{
			split_index_array.push_back(temp_split_index);
			temp_split_index = str_input.find_first_of(split_chrs_input[split_index], temp_split_index+1);
		}
	}

	sort(split_index_array.begin(), split_index_array.end());
	split_count = (int)split_index_array.size();
	temp_split_index = 0;
	for(int split_index=0; split_index<split_count; ++split_index)
	{
		if(temp_split_index < split_index_array[split_index])
		{
			CT2CA pszConvertedAnsiString(str_input_cstring.Mid(temp_split_index, split_index_array[split_index]-temp_split_index));
			string temp_split_str(pszConvertedAnsiString);
			temp_split_index = split_index_array[split_index];
			if(temp_split_str != "")
			{
				str_output.push_back(temp_split_str);
			}
		}
	}

	return (int)str_output.size();
}

void COptUtil::DrawAxis(CString axis_name)
{
}

double COptUtil::IGRIP_SSSA(double s1, double s2, double s3)
{
	// acos 으로 계산할 수 있는 범위를 벗어나면 에러 : -99;
	double val = (s2*s2 +s3*s3 -s1*s1) /(2*s2*s3);
	if(fabs(val) > 1)
		return -99;

	return acos(val);
}

double COptUtil::IGRIP_SASS(double s1, double a1, double s2)
{
	// sqrt로 계산할수 있는 범위를 벗어나면 에러 : -99;
	double val = s2*s2 +s1*s1 -2*s2*s1*cos(a1);
	if(val < 0)
		return -99;

	return sqrt(val);
}

double COptUtil::IGRIP_SASA(double s1, double a1, double s2)
{
	//double c_sass;

	//// sqrt로 계산할수 있는 범위를 벗어나면 에러 : -99;
	//double val = s2*s2 +s1*s1 -2*s2*s1*cos(a1);
	//if(val < 0)
	//	return -99;
	//c_sass = IGRIP_SASS(s1, a1, s2);

	//// acos 으로 계산할 수 있는 범위를 벗어나면 에러 : -99;
	//val = (s2*s2 +c_sass*c_sass -s1*s1) /(2*s2*c_sass);
	//if(fabs(val) > 1)
	//	return -99;

	//if(a1>=0)
	//	return IGRIP_SSSA(s1, s2, c_sass);
	//else
	//	return -IGRIP_SSSA(s1, s2, c_sass);
	double param1 = s2*sin(a1);
	double param2 = s1-s2*cos(a1);
	if(param2 == 0)
	{
		return -99;
	}
	return 180.0-a1-atan2(param1, param2);
}

CString COptUtil::IGRIP_SSSA(CString s1, CString s2, CString s3)
{
	CString return_string;
	return_string.Format(_T("acos(((((%s)*(%s))+((%s)*(%s))-((%s)*(%s)))/(2*(%s)*(%s))))"), 
		s2, s2, s3, s3, s1, s1, s2, s3);

	return return_string;
}

CString COptUtil::IGRIP_SASS(CString s1, CString a1, CString s2)
{
	CString return_string;
	return_string.Format(_T("sqrt((%s)*(%s)+(%s)*(%s)-(2*(%s)*(%s)*cos(%s)))"),
		s2, s2, s1, s1, s2, s1, a1);

	return return_string;
}

CString COptUtil::IGRIP_SASA(CString s1, CString a1, CString s2)
{
	CString return_string;
	return_string.Format(_T("rad(180-(%s)-atan2((%s)*sin((%s)),(%s)-(%s)*cos((%s))))"),
		a1, s2, a1, s1, s2, a1);

	return return_string;
	//return IGRIP_SSSA(s1, s2, IGRIP_SASS(s1, a1, s2));
}

OPoint3D::~OPoint3D(void)
{
}

OPoint2D::~OPoint2D(void)
{
}

IMPLEMENT_SERIAL(MATRIX44, CObject, 1)
MATRIX44::MATRIX44(void)
{
	Identity();
}

MATRIX44::~MATRIX44(void)
{
}

void MATRIX44::Identity()
{
	COptUtil::Identity_Matrix(m);
	m_bIdentity = true;
}

void MATRIX44::GetTr(float (*mat)[4])
{
	COptUtil::Copy_Matrix(m, mat);
}

void MATRIX44::SetTr(float (*mat)[4])
{
	COptUtil::Copy_Matrix(mat, m);
}

void MATRIX44::GetTr(double (*mat)[4])
{
	COptUtil::Copy_Matrix(m, mat);
}

void MATRIX44::SetTr(double (*mat)[4])
{
	COptUtil::Copy_Matrix(mat, m);
}

void MATRIX44::GetTr(MATRIX44& mat)
{
	COptUtil::Copy_Matrix(m, mat.m);
	m_bIdentity = mat.m_bIdentity;
}

void MATRIX44::SetTr(MATRIX44 mat)
{
	COptUtil::Copy_Matrix(mat.m, m);
	m_bIdentity = mat.m_bIdentity;
}

void MATRIX44::AppendMatrix(MATRIX44 mat)
{
	float origin[4][4];
	GetTr(origin);
	COptUtil::Compute_Matrix_Product(origin, mat.m, m);
	m_bIdentity = false;
}

void MATRIX44::AppendMatrix(float (*mat)[4])
{
	float origin[4][4];
	GetTr(origin);
	COptUtil::Compute_Matrix_Product(origin, mat, m);
	m_bIdentity = false;
}

void MATRIX44::AppendMatrix(double (*mat)[4])
{
	double origin[4][4];
	double temp_m[4][4];
	GetTr(origin);
	COptUtil::Compute_Matrix_Product(origin, mat, temp_m);
	SetTr(temp_m);
	m_bIdentity = false;
}

void MATRIX44::Tranfrom_Point(OPoint3D& in_point, OPoint3D& out_point)
{
	out_point.x =  m[0][0]*in_point.x + m[0][1]*in_point.y +m[0][2]*in_point.z + m[0][3];
	out_point.y =  m[1][0]*in_point.x + m[1][1]*in_point.y +m[1][2]*in_point.z + m[1][3];
	out_point.z =  m[2][0]*in_point.x + m[2][1]*in_point.y +m[2][2]*in_point.z + m[2][3];
}

void MATRIX44::Tranfrom_Point(OPoint3D* in_point, OPoint3D& out_point)
{
	out_point.x =  m[0][0]*in_point->x + m[0][1]*in_point->y +m[0][2]*in_point->z + m[0][3];
	out_point.y =  m[1][0]*in_point->x + m[1][1]*in_point->y +m[1][2]*in_point->z + m[1][3];
	out_point.z =  m[2][0]*in_point->x + m[2][1]*in_point->y +m[2][2]*in_point->z + m[2][3];
}

void MATRIX44::Tranfrom_Point(float x, float y, float z, OPoint3D& out_point)
{
	out_point.x =  m[0][0]*x + m[0][1]*y +m[0][2]*z + m[0][3];
	out_point.y =  m[1][0]*x + m[1][1]*y +m[1][2]*z + m[1][3];
	out_point.z =  m[2][0]*x + m[2][1]*y +m[2][2]*z + m[2][3];
}

OPoint3D MATRIX44::Tranfrom_Point(OPoint3D& in_point)
{
	OPoint3D out_point;
	out_point.x =  m[0][0]*in_point.x + m[0][1]*in_point.y +m[0][2]*in_point.z + m[0][3];
	out_point.y =  m[1][0]*in_point.x + m[1][1]*in_point.y +m[1][2]*in_point.z + m[1][3];
	out_point.z =  m[2][0]*in_point.x + m[2][1]*in_point.y +m[2][2]*in_point.z + m[2][3];
	return out_point;
}

void MATRIX44::GetInvTr(MATRIX44& mat)
{
	double temp_mat[4][4];
	double temp_inv_mat[4][4];
	GetTr(temp_mat);
	COptUtil::InverseMAT(temp_inv_mat, temp_mat);
	mat.SetTr(temp_inv_mat);
}

void MATRIX44::GetInvTr(float (*mat)[4])
{
	double temp_mat[4][4];
	double temp_inv_mat[4][4];
	GetTr(temp_mat);
	COptUtil::InverseMAT(temp_inv_mat, temp_mat);
	COptUtil::Copy_Matrix(temp_inv_mat, mat);
}

void MATRIX44::GetInvTr(double (*mat)[4])
{
	double temp_mat[4][4];
	GetTr(temp_mat);
	COptUtil::InverseMAT(mat, temp_mat);
}

void MATRIX44::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ar << m[i][j];
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ar >> m[i][j];
			}
		}
	}
}

MATRIX43::MATRIX43(void)
{
}

MATRIX43::~MATRIX43(void)
{
}

// x,y,z,yaw,pitch,roll => transformation matrix
void COptUtil::Make_Mat_YPR (double tx,double ty,double tz,double yaw,double pitch,double roll,double (*mat)[4])
{
	double sy,cy,sp,cp,sr,cr;

	sy =  sin(yaw);
	cy =  cos(yaw);
	sp =  sin(pitch);
	cp =  cos(pitch);
	sr =  sin(roll);
	cr =  cos(roll);

	mat[0][0] = cr*cp;
	mat[0][1] = cr*sp*sy - sr*cy;
	mat[0][2] = cr*sp*cy + sr*sy;
	mat[0][3] = tx;
	mat[1][0] = sr*cp;
	mat[1][1] = sr*sp*sy + cr*cy;
	mat[1][2] = sr*sp*cy - cr*sy;
	mat[1][3] = ty;
	mat[2][0] = -sp;
	mat[2][1] = cp*sy ;
	mat[2][2] = cp*cy;
	mat[2][3] = tz;
	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
}

// x,y,z,rx,ry,rz => transformation matrix
void COptUtil::Make_Mat_RXYZ(double tx,double ty, double tz, double rx, double ry, double rz, double (*mat)[4])
{

	double srx,crx,sry,cry,srz,crz;

	srx = sin(rx);
	crx = cos(rx);
	sry = sin(ry);
	cry = cos(ry);
	srz = sin(rz);
	crz = cos(rz);

	mat[0][0] = cry*crz;
	mat[1][0] = srx*sry*crz + crx*srz;
	mat[2][0] = -crx*sry*crz + srx*srz;
	mat[3][0] = 0;

	mat[0][1] = -cry*srz;
	mat[1][1] = -srx*sry*srz + crx*crz;
	mat[2][1] =  crx*sry*srz + srx*crz;
	mat[3][1] = 0;


	mat[0][2] = sry;
	mat[1][2] = -srx*cry;
	mat[2][2] = crx*cry;
	mat[3][2] = 0;

	mat[0][3] = tx;
	mat[1][3] = ty;
	mat[2][3] = tz;
	mat[3][3] = 1;
}


// transformation matrix => x,y,z,yaw,pitch,roll
int COptUtil::Make_Pos_YPR(double (*mat)[4], double *tx, double *ty, double *tz, double *yaw, double *pitch, double *roll)
{

	double r11,r12,r13,r21,r22,r23,r31,r32,r33;
	double cy,sy;

	r11 = mat[0][0];
	r12 = mat[0][1];
	r13 = mat[0][2];
	r21 = mat[1][0];
	r22 = mat[1][1];
	r23 = mat[1][2];
	r31 = mat[2][0];
	r32 = mat[2][1];
	r33 = mat[2][2];

	tx[0] = mat[0][3];
	ty[0] = mat[1][3];
	tz[0] = mat[2][3];


	cy = sqrt(r32*r32 + r33*r33);
	sy = -r31;
	if(fabs(cy) > 0){
		pitch[0] = atan2(sy,cy);
		roll[0] = atan2(r21/cy,r11/cy);
		yaw[0] = atan2(r32/cy,r33/cy);
	}
	else {
		if(sy>=0){
			pitch[0] = 1.57;
			roll[0]  = 0;
			yaw[0]   =atan2(r12,r22);
		}
		else{
			pitch[0] = -1.57;
			roll[0] = 0;
			yaw[0] = - atan2(r12,r22);
		}
	}
	return 0;
}

// transformation matrix => x,y,z,rx,ry,rz 
int COptUtil::Make_Pos_RXYZ(double (*mat)[4], double *tx, double *ty, double *tz, double *rx, double *ry, double *rz)
{

	double r11,r12,r13,r21,r22,r23,r31,r32,r33;
	double cx,cy,cz,sx,sy,sz;

	r11 = mat[0][0];
	r12 = mat[0][1];
	r13 = mat[0][2];
	r21 = mat[1][0];
	r22 = mat[1][1];
	r23 = mat[1][2];
	r31 = mat[2][0];
	r32 = mat[2][1];
	r33 = mat[2][2];

	tx[0] = mat[0][3];
	ty[0] = mat[1][3];
	tz[0] = mat[2][3];


	cy = sqrt(r11*r11 + r12*r12);
	sy = r13;
	if(fabs(cy) > 0) {
		//find ry
		ry[0] = atan2(sy,cy);

		//find rx
		sx = -r23/cy;
		cx = r33/cy;
		rx[0] = atan2(sx,cx);

		//find rz
		sz = -r12/cy;
		cz = r11/cy;
		rz[0] = atan2(sz,cz);

		return 0;
	}
	else{
		if(sy>0){
			ry[0] = 1.57;
			rx[0] = 0;
			rz[0] = atan2(r21,r23);	
			return 1;
		}
		else{
			ry[0] =-1.57;
			rx[0] = 0;
			rz[0] = -atan2(r21,r23);
			return 2;
		}
	}
}

// x,y,z,yaw,pitch,roll => transformation matrix
void COptUtil::Make_Mat_YPR (float tx,float ty,float tz,float yaw,float pitch,float roll,float (*mat)[4])
{
	float sy,cy,sp,cp,sr,cr;

	sy =  sin(yaw);
	cy =  cos(yaw);
	sp =  sin(pitch);
	cp =  cos(pitch);
	sr =  sin(roll);
	cr =  cos(roll);

	mat[0][0] = cr*cp;
	mat[0][1] = cr*sp*sy - sr*cy;
	mat[0][2] = cr*sp*cy + sr*sy;
	mat[0][3] = tx;
	mat[1][0] = sr*cp;
	mat[1][1] = sr*sp*sy + cr*cy;
	mat[1][2] = sr*sp*cy - cr*sy;
	mat[1][3] = ty;
	mat[2][0] = -sp;
	mat[2][1] = cp*sy ;
	mat[2][2] = cp*cy;
	mat[2][3] = tz;
	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
}

// x,y,z,rx,ry,rz => transformation matrix
void COptUtil::Make_Mat_RXYZ(float tx,float ty, float tz, float rx, float ry, float rz, float (*mat)[4])
{

	float srx,crx,sry,cry,srz,crz;

	srx = sin(rx);
	crx = cos(rx);
	sry = sin(ry);
	cry = cos(ry);
	srz = sin(rz);
	crz = cos(rz);

	mat[0][0] = cry*crz;
	mat[1][0] = srx*sry*crz + crx*srz;
	mat[2][0] = -crx*sry*crz + srx*srz;
	mat[3][0] = 0;

	mat[0][1] = -cry*srz;
	mat[1][1] = -srx*sry*srz + crx*crz;
	mat[2][1] =  crx*sry*srz + srx*crz;
	mat[3][1] = 0;


	mat[0][2] = sry;
	mat[1][2] = -srx*cry;
	mat[2][2] = crx*cry;
	mat[3][2] = 0;

	mat[0][3] = tx;
	mat[1][3] = ty;
	mat[2][3] = tz;
	mat[3][3] = 1;
}


// transformation matrix => x,y,z,yaw,pitch,roll
int COptUtil::Make_Pos_YPR(float (*mat)[4], float *tx, float *ty, float *tz, float *yaw, float *pitch, float *roll)
{

	float r11,r12,r13,r21,r22,r23,r31,r32,r33;
	float cy,sy;

	r11 = mat[0][0];
	r12 = mat[0][1];
	r13 = mat[0][2];
	r21 = mat[1][0];
	r22 = mat[1][1];
	r23 = mat[1][2];
	r31 = mat[2][0];
	r32 = mat[2][1];
	r33 = mat[2][2];

	tx[0] = mat[0][3];
	ty[0] = mat[1][3];
	tz[0] = mat[2][3];


	cy = sqrt(r32*r32 + r33*r33);
	sy = -r31;
	if(fabs(cy) > 0){
		pitch[0] = atan2(sy,cy);
		roll[0] = atan2(r21/cy,r11/cy);
		yaw[0] = atan2(r32/cy,r33/cy);
	}
	else {
		if(sy>=0){
			pitch[0] = 1.57f;
			roll[0]  = 0;
			yaw[0]   =atan2(r12,r22);
		}
		else{
			pitch[0] = -1.57f;
			roll[0] = 0;
			yaw[0] = - atan2(r12,r22);
		}
	}
	return 0;
}

// transformation matrix => x,y,z,rx,ry,rz 
int COptUtil::Make_Pos_RXYZ(float (*mat)[4], float *tx, float *ty, float *tz, float *rx, float *ry, float *rz)
{

	float r11,r12,r13,r21,r22,r23,r31,r32,r33;
	float cx,cy,cz,sx,sy,sz;

	r11 = mat[0][0];
	r12 = mat[0][1];
	r13 = mat[0][2];
	r21 = mat[1][0];
	r22 = mat[1][1];
	r23 = mat[1][2];
	r31 = mat[2][0];
	r32 = mat[2][1];
	r33 = mat[2][2];

	tx[0] = mat[0][3];
	ty[0] = mat[1][3];
	tz[0] = mat[2][3];


	cy = sqrt(r11*r11 + r12*r12);
	sy = r13;
	if(fabs(cy) > 0) {
		//find ry
		ry[0] = atan2(sy,cy);

		//find rx
		sx = -r23/cy;
		cx = r33/cy;
		rx[0] = atan2(sx,cx);

		//find rz
		sz = -r12/cy;
		cz = r11/cy;
		rz[0] = atan2(sz,cz);

		return 0;
	}
	else{
		if(sy>0){
			ry[0] = 1.57f;
			rx[0] = 0;
			rz[0] = atan2(r21,r23);	
			return 1;
		}
		else{
			ry[0] =-1.57f;
			rx[0] = 0;
			rz[0] = -atan2(r21,r23);
			return 2;
		}
	}
}

void COptUtil::Compute_Matrix_Product (double (*mat1)[4], double (*mat2)[4], double (*out_mat)[4])
////////////////////////////// eZRobotics ////////////////////////////////////////
{
	int i, j, k;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++) {
			out_mat[i][j] = 0.;
			for (k=0; k<4; k++)
				out_mat[i][j] += (mat1[i][k] * mat2[k][j]);
		}
}

void COptUtil::Compute_Matrix_Product (float (*mat1)[4], float (*mat2)[4], float (*out_mat)[4])
////////////////////////////// eZRobotics ////////////////////////////////////////
{
	int i, j, k;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++) {
			out_mat[i][j] = 0.;
			for (k=0; k<4; k++)
				out_mat[i][j] += (mat1[i][k] * mat2[k][j]);
		}
}

void COptUtil::Compute_Matrix_Inverse (double (*in_mat)[4], double (*out_mat)[4])
////////////////////////////// eZRobotics ////////////////////////////////////////
{
	Copy_Matrix(in_mat, out_mat);
}

void COptUtil::Compute_Matrix_Inverse (float (*in_mat)[4], float (*out_mat)[4])
////////////////////////////// eZRobotics ////////////////////////////////////////
{
	Copy_Matrix(in_mat, out_mat);
}

void COptUtil::Copy_Matrix(double (*in_mat)[4], double (*out_mat)[4])
{
	out_mat[0][0] = in_mat[0][0]; 	out_mat[0][1] = in_mat[0][1]; 	out_mat[0][2] = in_mat[0][2]; 	out_mat[0][3] = in_mat[0][3];
	out_mat[1][0] = in_mat[1][0]; 	out_mat[1][1] = in_mat[1][1]; 	out_mat[1][2] = in_mat[1][2]; 	out_mat[1][3] = in_mat[1][3];
	out_mat[2][0] = in_mat[2][0]; 	out_mat[2][1] = in_mat[2][1]; 	out_mat[2][2] = in_mat[2][2]; 	out_mat[2][3] = in_mat[2][3];
	out_mat[3][0] = in_mat[3][0]; 	out_mat[3][1] = in_mat[3][1]; 	out_mat[3][2] = in_mat[3][2]; 	out_mat[3][3] = in_mat[3][3];
}

void COptUtil::Copy_Matrix(float (*in_mat)[4], float (*out_mat)[4])
{
	out_mat[0][0] = in_mat[0][0]; 	out_mat[0][1] = in_mat[0][1]; 	out_mat[0][2] = in_mat[0][2]; 	out_mat[0][3] = in_mat[0][3];
	out_mat[1][0] = in_mat[1][0]; 	out_mat[1][1] = in_mat[1][1]; 	out_mat[1][2] = in_mat[1][2]; 	out_mat[1][3] = in_mat[1][3];
	out_mat[2][0] = in_mat[2][0]; 	out_mat[2][1] = in_mat[2][1]; 	out_mat[2][2] = in_mat[2][2]; 	out_mat[2][3] = in_mat[2][3];
	out_mat[3][0] = in_mat[3][0]; 	out_mat[3][1] = in_mat[3][1]; 	out_mat[3][2] = in_mat[3][2]; 	out_mat[3][3] = in_mat[3][3];
}

void COptUtil::Copy_Matrix(double (*in_mat)[4], float (*out_mat)[4])
{
	out_mat[0][0] = in_mat[0][0]; 	out_mat[0][1] = in_mat[0][1]; 	out_mat[0][2] = in_mat[0][2]; 	out_mat[0][3] = in_mat[0][3];
	out_mat[1][0] = in_mat[1][0]; 	out_mat[1][1] = in_mat[1][1]; 	out_mat[1][2] = in_mat[1][2]; 	out_mat[1][3] = in_mat[1][3];
	out_mat[2][0] = in_mat[2][0]; 	out_mat[2][1] = in_mat[2][1]; 	out_mat[2][2] = in_mat[2][2]; 	out_mat[2][3] = in_mat[2][3];
	out_mat[3][0] = in_mat[3][0]; 	out_mat[3][1] = in_mat[3][1]; 	out_mat[3][2] = in_mat[3][2]; 	out_mat[3][3] = in_mat[3][3];
}

void COptUtil::Copy_Matrix(float (*in_mat)[4], double (*out_mat)[4])
{
	out_mat[0][0] = in_mat[0][0]; 	out_mat[0][1] = in_mat[0][1]; 	out_mat[0][2] = in_mat[0][2]; 	out_mat[0][3] = in_mat[0][3];
	out_mat[1][0] = in_mat[1][0]; 	out_mat[1][1] = in_mat[1][1]; 	out_mat[1][2] = in_mat[1][2]; 	out_mat[1][3] = in_mat[1][3];
	out_mat[2][0] = in_mat[2][0]; 	out_mat[2][1] = in_mat[2][1]; 	out_mat[2][2] = in_mat[2][2]; 	out_mat[2][3] = in_mat[2][3];
	out_mat[3][0] = in_mat[3][0]; 	out_mat[3][1] = in_mat[3][1]; 	out_mat[3][2] = in_mat[3][2]; 	out_mat[3][3] = in_mat[3][3];
}

void COptUtil::Identity_Matrix(float(*m)[4])
{
	m[0][0] = 1.0; 	m[0][1] = 0.0; 	m[0][2] = 0.0; 	m[0][3] = 0.0;
	m[1][0] = 0.0; 	m[1][1] = 1.0; 	m[1][2] = 0.0; 	m[1][3] = 0.0;
	m[2][0] = 0.0; 	m[2][1] = 0.0; 	m[2][2] = 1.0; 	m[2][3] = 0.0;
	m[3][0] = 0.0; 	m[3][1] = 0.0; 	m[3][2] = 0.0; 	m[3][3] = 1.0;
}

void COptUtil::Identity_Matrix(double(*m)[4])
{
	m[0][0] = 1.0; 	m[0][1] = 0.0; 	m[0][2] = 0.0; 	m[0][3] = 0.0;
	m[1][0] = 0.0; 	m[1][1] = 1.0; 	m[1][2] = 0.0; 	m[1][3] = 0.0;
	m[2][0] = 0.0; 	m[2][1] = 0.0; 	m[2][2] = 1.0; 	m[2][3] = 0.0;
	m[3][0] = 0.0; 	m[3][1] = 0.0; 	m[3][2] = 0.0; 	m[3][3] = 1.0;
}

MATRIX COptUtil::_mat_creat( int row, int col )
{
	MATBODY	*mat;
	int 	i, j;

	if ((mat = (MATBODY *)malloc( sizeof(MATHEAD) + sizeof(double *) * row)) == NULL)
		return (NULL);

	for (i=0; i<row; i++)
	{
	if ((*((double **)(&mat->matrix) + i) = (double *)malloc(sizeof(double) * col)) == NULL)
		return (NULL);
	}

	mat->head.row = row;
	mat->head.col = col;

	return (&(mat->matrix));
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_creat
*	desct:	create a matrix
*	given:  row, col = dimension, type = which kind of matrix
*	retrn:	allocated matrix (use mat_free() to free memory)
*-----------------------------------------------------------------------------
*/
MATRIX	COptUtil::mat_creat( int row, int col, int type )
{
	MATRIX	A;

	if ((A =_mat_creat( row, col )) != NULL)
		{
		return (mat_fill(A, type));
		}
	else
		return (NULL);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_fill
*	desct:	form a special matrix
*	given:  A = matrix, type = which kind of matrix
*	retrn:	A
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_fill( MATRIX A, int type )
{
	int	i, j;

	switch (type)
		{
		case UNDEFINED:
			break;
		case ZERO_MATRIX:
		case UNIT_MATRIX:
			for (i=0; i<MatRow(A); i++)
			for (j=0; j<MatCol(A); j++)
				{
				if (type == UNIT_MATRIX)
					{
					if (i==j)
						{
						A[i][j] = 1.0;
						continue;
						}
					}
				A[i][j] = 0.0;
				}
			break;
		}
	return (A);
}


/*
*-----------------------------------------------------------------------------
*	funct:	mat_free
*	desct:	free an allocated matrix
*	given:  A = matrix
*	retrn:	nothing <actually 0 = NULL A passed, 1 = normal exit>
*-----------------------------------------------------------------------------
*/
int COptUtil::mat_free( MATRIX A )
{
	int i;

	if (A == NULL)
		return (0);
	for (i=0; i<MatRow(A); i++)
		{
		free( A[i] );
		}
	free( Mathead(A) );
	return (1);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_copy
*	desct:	duplicate a matrix
*	given:	A = matrice to duplicated
*	retrn:	C = A
*	comen:
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_copy( MATRIX A )
{
	int	i, j;
	MATRIX	C;

	if ((C = mat_creat( MatRow(A), MatCol(A), UNDEFINED )) == NULL)
		return (NULL);

	for (i=0; i<MatRow(A); i++)
	for (j=0; j<MatCol(A); j++)
		{
		C[i][j] = A[i][j];
		}
	return (C);
}


MATRIX COptUtil::mat_colcopy1( MATRIX A, MATRIX B, int cola, int colb )
{
	int	i, n;

	n = MatRow(A);
	for (i=0; i<n; i++)
		{
		A[i][cola] = B[i][colb];
		}
	return (A);
}

int COptUtil::fgetmat( MATRIX A, FILE *fp )
{
	int 	i, j, k=0;

	for (i=0; i<MatRow(A); i++)
	for (j=0; j<MatCol(A); j++)
		{
/*
*	to avoid a bug in TC
*/
#ifdef	__TURBOC__
		{
		double	temp;
		k += fscanf( fp, "%lf", &temp );
		A[i][j] = temp;
		}
#else
		k += fscanf_s( fp, "%lf", &A[i][j] );
#endif

		}

	return (k);
}

MATRIX COptUtil::mat_inv( MATRIX a )
{
	MATRIX	A, B, C, P;
	int	i, j, n;
	double	temp;

	n = MatCol(a);
	A = mat_copy(a);
	B = mat_creat( n, 1, UNDEFINED );
	C = mat_creat( n, n, UNDEFINED );
	P = mat_creat( n, 1, UNDEFINED );

	/*
	*	- LU-decomposition -
	*	also check for singular matrix
	*/
	if (mat_lu(A, P) == -1)
		{
		mat_free(A);
		mat_free(B);
		mat_free(C);
		mat_free(P);

		return (NULL);
		}

	for (i=0; i<n; i++)
		{
		mat_fill(B, ZERO_MATRIX);
		B[i][0] = 1.0;
		mat_backsubs1( A, B, C, P, i );
		}

	mat_free(A);
	mat_free(B);
	mat_free(P);
	return (C);
}

double COptUtil::mat_minor( MATRIX A, int i, int j )
{
	MATRIX	S;
	double	result;

	S = mat_submat(A, i, j);
	result = mat_det( S );
	mat_free(S);

	return (result);

}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_cofact
*	desct:	find cofactor
*	given:	A = a square matrix,
*		i=row, j=col
*	retrn:	the cofactor of Aij
*-----------------------------------------------------------------------------
*/
double COptUtil::mat_cofact( MATRIX A, int i, int j )
{
	double	result;

	result = signa[(i+j)%2] * A[i][j] * mat_minor(A, i, j);

	return (result);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_det
*	desct:	find determinant
*	given:	A = matrix
*	retrn:	the determinant of A
*	comen:
*-----------------------------------------------------------------------------
*/
double COptUtil::mat_det( MATRIX a )
{
	MATRIX	A, P;
	int	i, j, n;
	double	result;

	n = MatRow(a);
	A = mat_copy(a);
	P = mat_creat(n, 1, UNDEFINED);

	/*
	* take a LUP-decomposition
	*/
	i = mat_lu(A, P);
	switch (i)
		{
		/*
		* case for singular matrix
		*/
		case -1:
		result = 0.0;
		break;

		/*
		* normal case: |A| = |L||U||P|
		* |L| = 1,
		* |U| = multiplication of the diagonal
		* |P| = +-1
		*/
		default:
		result = 1.0;
		for (j=0; j<MatRow(A); j++)
			{
			result *= A[(int)P[j][0]][j];
			}
		result *= signa[i%2];
		break;
		}

	mat_free(A);
	mat_free(P);
	return (result);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_lu
*	desct:	in-place LU decomposition with partial pivoting
*	given:	!! A = square matrix (n x n) !ATTENTION! see commen
*		P = permutation vector (n x 1)
*	retrn:	number of permutation performed
*		-1 means suspected singular matrix
*	comen:	A will be overwritten to be a LU-composite matrix
*
*	note:	the LU decomposed may NOT be equal to the LU of
*		the orignal matrix a. But equal to the LU of the
*		rows interchanged matrix.
*-----------------------------------------------------------------------------
*/
int COptUtil::mat_lu( MATRIX A, MATRIX P )
{
	int	i, j, k, n;
	int	maxi, tmp;
	double	c, c1;
	int	p;

	n = MatCol(A);

	for (p=0,i=0; i<n; i++)
		{
		P[i][0] = i;
		}

	for (k=0; k<n; k++)
	{
	/*
	* --- partial pivoting ---
	*/
	for (i=k, maxi=k, c=0.0; i<n; i++)
		{
		c1 = fabs( A[(int)P[i][0]][k] );
		if (c1 > c)
			{
			c = c1;
			maxi = i;
			}
		}

	/*
	*	row exchange, update permutation vector
	*/
	if (k != maxi)
		{
		p++;
		tmp = P[k][0];
		P[k][0] = P[maxi][0];
		P[maxi][0] = tmp;
		}

	/*
	*	suspected singular matrix
	*/
	if ( A[(int)P[k][0]][k] == 0.0 )
		return (-1);

	for (i=k+1; i<n; i++)
		{
		/*
		* --- calculate m(i,j) ---
		*/
		A[(int)P[i][0]][k] = A[(int)P[i][0]][k] / A[(int)P[k][0]][k];

		/*
		* --- elimination ---
		*/
		for (j=k+1; j<n; j++)
			{
			A[(int)P[i][0]][j] -= A[(int)P[i][0]][k] * A[(int)P[k][0]][j];
			}
		}
	}

	return (p);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_backsubs1
*	desct:	back substitution
*	given:	A = square matrix A (LU composite)
*		!! B = column matrix B (attention!, see comen)
*		!! X = place to put the result of X
*		P = Permutation vector (after calling mat_lu)
*		xcol = column of x to put the result
*	retrn:	column matrix X (of AX = B)
*	comen:	B will be overwritten
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_backsubs1( MATRIX A, MATRIX B, MATRIX X, MATRIX P, int xcol )
{
	int	i, j, k, n;
	double	sum;

	n = MatCol(A);

	for (k=0; k<n; k++)
		{
		for (i=k+1; i<n; i++)
			B[(int)P[i][0]][0] -= A[(int)P[i][0]][k] * B[(int)P[k][0]][0];
		}

	X[n-1][xcol] = B[(int)P[n-1][0]][0] / A[(int)P[n-1][0]][n-1];
	for (k=n-2; k>=0; k--)
		{
		sum = 0.0;
		for (j=k+1; j<n; j++)
			{
			sum += A[(int)P[k][0]][j] * X[j][xcol];
			}
		X[k][xcol] = (B[(int)P[k][0]][0] - sum) / A[(int)P[k][0]][k];
		}

	return (X);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_lsolve
*	desct:	solve linear equations
*	given:	a = square matrix A
*		b = column matrix B
*	retrn:	column matrix X (of AX = B)
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_lsolve( MATRIX a, MATRIX b )
{
	MATRIX	A, B, X, P;
	int	i, n;
	double	temp;

	n = MatCol(a);
	A = mat_copy(a);
	B = mat_copy(b);
	X = mat_creat(n, 1, ZERO_MATRIX);
	P = mat_creat(n, 1, UNDEFINED);

	mat_lu( A, P );
	mat_backsubs1( A, B, X, P, 0 );

	mat_free(A);
	mat_free(B);
	mat_free(P);
	return (X);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_add
*	desct:	addition of two matrice
*	given:	A, B = Compatible matrice to be added
*	retrn:	NULL if malloc() fails
*		else allocated matrix of A + B
*	comen:
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_add( MATRIX A, MATRIX B )
{
	int	i, j;
	MATRIX	C;

	if ((C = mat_creat( MatRow(A), MatCol(A), UNDEFINED )) == NULL)
		return (NULL);

	for (i=0; i<MatRow(A); i++)
	for (j=0; j<MatCol(A); j++)
		{
		C[i][j] = A[i][j] + B[i][j];
		}
	return (C);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_durbin
*	desct:	Levinson-Durbin algorithm
*
*		This function solve the linear eqns Ax = B:
*
*		|  v0   v1   v2  .. vn-1 | |  a1   |    |  v1   |
*		|  v1   v0   v1  .. vn-2 | |  a2   |    |  v2   |
*		|  v2   v1   v0  .. vn-3 | |  a3   |  = |  ..   |
*		|  ...                   | |  ..   |    |  ..   |
*		|  vn-1 vn-2 ..  .. v0   | |  an   |    |  vn   |
*
*		where A is a symmetric Toeplitz matrix and B
*		in the above format (related to A)
*
*	given:	R = autocorrelated matrix (v0, v1, ... vn) (dim (n+1) x 1)
*	retrn:	x (of Ax = B)
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_durbin( MATRIX R )
{
	int	i, i1, j, ji, p, n;
	MATRIX	W, E, K, A, X;

	p = MatRow(R) - 1;
	W = mat_creat( p+2, 1, UNDEFINED );
	E = mat_creat( p+2, 1, UNDEFINED );
	K = mat_creat( p+2, 1, UNDEFINED );
	A = mat_creat( p+2, p+2, UNDEFINED );

	W[0][0] = R[1][0];
	E[0][0] = R[0][0];

	for (i=1; i<=p; i++)
		{
		K[i][0] = W[i-1][0] / E[i-1][0];
		E[i][0] = E[i-1][0] * (1.0 - K[i][0] * K[i][0]);

		A[i][i] = -K[i][0];

		i1 = i-1;
		if (i1 >= 1)
			{
			for (j=1; j<=i1; j++)
				{
				ji = i - j;
				A[j][i] = A[j][i1] - K[i][0] * A[ji][i1];
				}
			}

		if (i != p)
			{
			W[i][0] = R[i+1][0];
			for (j=1; j<=i; j++)
				W[i][0] += A[j][i] * R[i-j+1][0];
			}
		}

	X = mat_creat( p, 1, UNDEFINED );
	for (i=0; i<p; i++)
		{
		X[i][0] = -A[i+1][p];
		}

	mat_free( A );
	mat_free( W );
	mat_free( K );
	mat_free( E );
	return (X);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_lsolve_durbin
*	desct:	Solve simultaneous linear eqns using
*		Levinson-Durbin algorithm
*
*		This function solve the linear eqns Ax = B:
*
*		|  v0   v1   v2  .. vn-1 | |  a1   |    |  v1   |
*		|  v1   v0   v1  .. vn-2 | |  a2   |    |  v2   |
*		|  v2   v1   v0  .. vn-3 | |  a3   |  = |  ..   |
*		|  ...                   | |  ..   |    |  ..   |
*		|  vn-1 vn-2 ..  .. v0   | |  an   |    |  vn   |
*
*	domain:	where A is a symmetric Toeplitz matrix and B
*		in the above format (related to A)
*
*	given:	A, B
*	retrn:	x (of Ax = B)
*
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_lsolve_durbin( MATRIX A, MATRIX B )
{
	MATRIX	R, X;
	int	i, n;

	n = MatRow(A);
	R = mat_creat(n+1, 1, UNDEFINED);
	for (i=0; i<n; i++)
		{
		R[i][0] = A[i][0];
		}
	R[n][0] = B[n-1][0];

	X = mat_durbin( R );
	mat_free( R );
	return (X);
}

MATRIX COptUtil:: mat_error( int errno )
{
	switch( errno )
		{
		case MAT_MALLOC:
			fprintf(stderr, "mat: malloc error\n" );
			break;
		case MAT_FNOTOPEN:
			fprintf(stderr, "mat: fileopen error\n" );
			break;
		case MAT_FNOTGETMAT:
			fprintf(stderr, "fgetmat: matrix read error\n");
			break;
		}

	return (NULL);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_mul
*	desct:	multiplication of two matrice
*	given:	A, B = compatible matrice to be multiplied
*	retrn:	NULL if malloc() fails
*		else allocated matrix of A * B
*	comen:
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil:: mat_mul( MATRIX A, MATRIX B )
{
	int	i, j, k;
	MATRIX	C;

	if ((C = mat_creat( MatRow(A), MatCol(B), UNDEFINED )) == NULL)
		return (NULL);

	for (i=0; i<MatRow(A); i++)
	for (j=0; j<MatCol(B); j++)
	for (k=0, C[i][j]=0.0; k<MatCol(A); k++)
		{
		C[i][j] += A[i][k] * B[k][j];
		}
	return (C);
}

double COptUtil:: mat_diagmul( MATRIX A )
{
	int i;
	double result = 1.0;

	for (i=0; i<MatRow(A); i++)
		{
		result *= A[i][i];
		}
	return (result);
}



/*
*-----------------------------------------------------------------------------
*	funct:	mat_sub
*	desct:	subtraction of two matrice
*	given:	A, B = compatible matrice to be added
*	retrn:	NULL if malloc() fails
*		else allocated matrix of A - B
*	comen:
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil:: mat_sub( MATRIX A, MATRIX B )
{
	int	i, j;
	MATRIX	C;

	if ((C = mat_creat( MatRow(A), MatCol(A), UNDEFINED )) == NULL)
		return (NULL);

	for (i=0; i<MatRow(A); i++)
	for (j=0; j<MatCol(A); j++)
		{
		C[i][j] = A[i][j] - B[i][j];
		}
	return (C);
}
/*
*-----------------------------------------------------------------------------
*	funct:	mat_submat
*	desct:	return a submatrix S of A
*	given:	A = main matrix,
*		i,j = row and column of A to be deleted to obtained S
*	retrn:	S
*-----------------------------------------------------------------------------
*/
MATRIX COptUtil::mat_submat( MATRIX A, int i, int j )
{
	int	m, m1, p, p1;
	MATRIX	S;

	S = mat_creat(MatRow(A)-1, MatCol(A)-1, UNDEFINED);

	for (m=m1=0; m<MatRow(A); m++)
		{
		if (m==i) continue;
		for (p=p1=0; p<MatCol(A); p++)
			{
			if (p==j) continue;
			S[m1][p1] = A[m][p];
			p1++;
			}
		m1++;
		}

	return (S);
}

/*
*-----------------------------------------------------------------------------
*	funct:	mat_SymToeplz
*	desct:	create a n x n symmetric Toeplitz matrix from
*		a n x 1 correlation matrix
*	given:	R = correlation matrix (n x 1)
*	retrn:	the symmetric Toeplitz matrix
*-----------------------------------------------------------------------------
*/
//MATRIX COptUtil::mat_SymToeplz( MATRIX R )
//{
//	int	i, j, n;
//	MATRIX	T;
//
//	n = MatRow(R);
//	T = mat_creat(n, n, UNDEFINED);
//
//	for (i=0; i<n; i++)
//	for (j=0; j<n; j++)
//		{
//		T[i][j] = R[abs(i-j)][0];
//		}
//	return (T);
//}
//
///*
//*-----------------------------------------------------------------------------
//*	funct:	mat_tran
//*	desct:	transpose of a matrix
//*	given:	A = matrix A to be transposed
//*	retrn:	allocated matrix for A^t
//*	comen:
//*-----------------------------------------------------------------------------
//*/
//MATRIX COptUtil::mat_tran( MATRIX A )
//{
//	int	i, j;
//	MATRIX	At;
//
//	if ((At = mat_creat( MatCol(A), MatRow(A), UNDEFINED )) == NULL)
//		return (NULL);
//
//	/*
//	*	Transposing ...
//	*/
//	for (i=0; i<MatCol(A); i++)
//	for (j=0; j<MatRow(A); j++)
//		{
//		At[i][j] = A[j][i];
//		}
//	return (At);
//}

//int COptUtil::Type_S_Inverse_Kin(
//	double mat[4][4],		//TCP 값
//	int config[3],			//config 값
//	double aa[6],			//
//	double dd[6],			//
//	int sign[6],			//joint 회전 방향 값
//	double jointOffset[6],	//joint 회전 offset 값
//	double alpha,			//5축 offset alpha 값
//	double deltaX[6],		//Calibration delta vlaue
//	double deltaY[6], 
//	double deltaZ[6], 
//	double deltaW[6], 
//	double deltaP[6], 
//	double deltaR[6], 
//	double theta[6])		//theta 값
//{
//	int i, j, k, l;
//	int iteration = 0;
//	int FindThetaFlag = 0;
//	
//	//config value
//	int shoulder, elbow, wrist;
//
//	double de_pi = PI/180;
//	double pi_de = 180/PI;
//
//	double temp_1[4][4], temp_2[4][4], temp_3[4][4], temp_4[4][4];
//	double temp_5[4][4] = {0};
//	double temp_6[4][4] = {0};
//	
//	//delta value matrices
//	double delta[6][4][4] = {0};
//	
//	//각 Joint의 delta vector matrices
//	double A[6], B[6], D[6], E[6];
//	double F[6], G[6], H[6], I[6];
//	double J[6], K[6], L[6], M[6];
//
//	double nx, ny, nz, ox, oy, oz, ax, ay, az, px, py, pz;//입력 받을 6축 좌표
//	double CosTheta[6] = {0};//cos theta값
//	double SinTheta[6] = {0};//sin theta값
//	double thetaPrev[6] = {0};
//	double Tolerance[6] = {0};
//
//	double Init_a5, Init_d5;
//
//	double AA1, AA2, AA3, AA4, AB1, AB2, AD1, AD2;
//	double AE1, AE2, AE3, AE4, AF1, AF2, AG1, AG2;
//	double AH1, AH2, AH3, AH4, AI1, AI2, AJ1, AJ2;
//	double BA1, BA2, BA3, BA4, DA1, DA2, DA3, DA4;
//	double DB1, DB2, DB3, DD1, DD2, DD3, DE1, DE2, DE3, DE4;
//	double DF1, DF2, DF3, DG1, DG2, DG3, DH1, DH2, DH3, DH4;
//	double DI1, DI2, DI3, DJ1, DJ2, DJ3;
//	double EA1, EA2, EA3, EA4, EA5, EA6, EA7, EA8;
//	double FA1, FA2, FA3, FA4, FA5, FA6;
//	double FB1, FB2, FB3, FB4, FB5, FB6, FB7, FB8, FB9;
//	double FD1, FD2, FD3, FD4, FD5, FD6, FD7, FD8;
//	double FE1, FE2, FE3, FE4;
//	double FH1, FH2, FH3, FH4, FH5, FH6;
//	double FI1, FI2, FI3, FI4, FI5, FI6, FI7, FI8, FI9;
//	double FJ1, FJ2, FJ3, FJ4, FJ5, FJ6;
//	double FK1, FK2, FK3, FK4, FK5, FK6, FK7, FK8, FK9;
//	double GA1, GA2, GA3, GA4, GA5, GA6, GA7, GA8, GB1, GB2, GB3, GB4;
//	double GD1, GD2, GD3, GD4, GD5, GD6, GE1, GE2, GE3, GE4, GE5, GE6, GE7, GE8;
//	double GF1, GF2, GF3, GF4;
//	double GG1, GG2, GG3, GG4, GG5, GG6, GH1, GH2, GH3, GH4, GH5, GH6, GH7, GH8;
//	double GI1, GI2, GI3, GI4, GJ1, GJ2, GJ3, GJ4, GJ5, GJ6;
//	double HA1, HA2, HA3, HA4, HA5, HA6;
//	double IA1, IA2, IA3, IA4, IA5, IA6, IB1, IB2, IB3, IB4, IB5, IB6, ID1, ID2;
//
//	double nx03, ny03, nz03, ox03, oy03, oz03, ax03, ay03, az03, pz26;
//	double KA1, KA2, KA3, KA4, KA5;
//	double LA1, LA2, LA3, LB1, LB2, LB3;
//	double JA1, JA2, JA3, JA4, JA5;
//
//	double PP, QQ, RR, PQR;
//	char test[1000];
//	//delta value matrices calculate
//	for(i=0; i<6; i++)
//	{
//		temp_1[0][0] = 1, temp_1[0][1] = 0, temp_1[0][2] = 0, temp_1[0][3] = deltaX[i];
//		temp_1[1][0] = 0, temp_1[1][1] = 1, temp_1[1][2] = 0, temp_1[1][3] = deltaY[i];
//		temp_1[2][0] = 0, temp_1[2][1] = 0, temp_1[2][2] = 1, temp_1[2][3] = deltaZ[i];
//		temp_1[3][0] = 0, temp_1[3][1] = 0, temp_1[3][2] = 0, temp_1[3][3] = 1;
//
//		temp_2[0][0] = cos(deltaR[i]*de_pi), temp_2[0][1] = -sin(deltaR[i]*de_pi), temp_2[0][2] = 0, temp_2[0][3] = 0;
//		temp_2[1][0] = sin(deltaR[i]*de_pi), temp_2[1][1] = cos(deltaR[i]*de_pi), temp_2[1][2] = 0, temp_2[1][3] = 0;
//		temp_2[2][0] = 0, temp_2[2][1] = 0, temp_2[2][2] = 1, temp_2[2][3] = 0;
//		temp_2[3][0] = 0, temp_2[3][1] = 0, temp_2[3][2] = 0, temp_2[3][3] = 1;
//
//		temp_3[0][0] = cos(deltaP[i]*de_pi), temp_3[0][1] = 0, temp_3[0][2] = sin(deltaP[i]*de_pi), temp_3[0][3] = 0;
//		temp_3[1][0] = 0, temp_3[1][1] = 1, temp_3[1][2] = 0, temp_3[1][3] = 0;
//		temp_3[2][0] = -sin(deltaP[i]*de_pi), temp_3[2][1] = 0, temp_3[2][2] = cos(deltaP[i]*de_pi), temp_3[2][3] = 0;
//		temp_3[3][0] = 0, temp_3[3][1] = 0, temp_3[3][2] = 0, temp_3[3][3] = 1;
//
//		temp_4[0][0] = 1, temp_4[0][1] = 0, temp_4[0][2] = 0, temp_4[0][3] = 0;
//		temp_4[1][0] = 0, temp_4[1][1] = cos(deltaW[i]*de_pi), temp_4[1][2] = -sin(deltaW[i]*de_pi), temp_4[1][3] = 0;
//		temp_4[2][0] = 0, temp_4[2][1] = sin(deltaW[i]*de_pi), temp_4[2][2] = cos(deltaW[i]*de_pi), temp_4[2][3] = 0;
//		temp_4[3][0] = 0, temp_4[3][1] = 0, temp_4[3][2] = 0, temp_4[3][3] = 1;
//
//		for(j=0; j<4; j++)
//		{
//			for(k=0; k<4; k++)
//			{
//				for(l=0; l<4; l++)
//				{
//					temp_5[j][k] = temp_5[j][k] + temp_1[j][l] * temp_2[l][k];
//				}
//			}
//		}
//		for(j=0; j<4; j++)
//		{
//			for(k=0; k<4; k++)
//			{
//				for(l=0; l<4; l++)
//				{
//					temp_6[j][k] = temp_6[j][k] + temp_5[j][l] * temp_3[l][k];
//				}
//			}
//		}
//		for(j=0; j<4; j++)
//		{
//			for(k=0; k<4; k++)
//			{
//				for(l=0; l<4; l++)
//				{
//					delta[i][j][k] = delta[i][j][k] + temp_6[j][l] * temp_4[l][k];
//				}
//			}
//		}
//		for(j=0; j<4; j++)
//		{
//			for(k=0; k<4; k++)
//			{
//				temp_5[j][k] = 0;
//				temp_6[j][k] = 0;
//			}
//		}
//	}
//
//	//각 조인트의 delta value
//	for(i=0; i<6; i++)
//	{
//		A[i] = delta[i][0][0];
//		B[i] = delta[i][0][1];
//		D[i] = delta[i][0][2];
//		E[i] = delta[i][0][3];
//		
//		F[i] = delta[i][1][0];
//		G[i] = delta[i][1][1];
//		H[i] = delta[i][1][2];
//		I[i] = delta[i][1][3];
//		
//		J[i] = delta[i][2][0];
//		K[i] = delta[i][2][1];
//		L[i] = delta[i][2][2];
//		M[i] = delta[i][2][3];
//	}
///*
//	for(i=0; i<6; i++)
//	{
//		sprintf(test, "A[%d] = %8.4lf B[%d] = %8.4lf D[%d] = %8.4lf E[%d] = %8.4lf\nF[%d] = %8.4lf G[%d] = %8.4lf H[%d] = %8.4lf I[%d] = %8.4lf\nJ[%d] = %8.4lf K[%d] = %8.4lf L[%d] = %8.4lf M[%d] = %8.4lf\n"
//			, i, A[i], i, B[i], i, D[i], i, E[i]
//			, i, F[i], i, G[i], i, H[i], i, I[i]
//			, i, J[i], i, K[i], i, L[i], i, M[i]);
//		MessageBox(NULL, test, "caution", MB_OK);
//	}*/
//
//	BA1 = -F[4]*cos(alpha*de_pi)+J[4]*sin(alpha*de_pi);	BA2 = -G[4]*cos(alpha*de_pi)+K[4]*sin(alpha*de_pi);
//	BA3 = -J[4]*cos(alpha*de_pi)-F[4]*sin(alpha*de_pi);	BA4 = -K[4]*cos(alpha*de_pi)-G[4]*sin(alpha*de_pi);
//
//	EA1 = F[5]*cos(alpha*de_pi)+J[5]*sin(alpha*de_pi);	EA2 = G[5]*cos(alpha*de_pi)+K[5]*sin(alpha*de_pi);
//	EA3 = J[5]*cos(alpha*de_pi)-F[5]*sin(alpha*de_pi);	EA4 = K[5]*cos(alpha*de_pi)-G[5]*sin(alpha*de_pi);
//
//	EA5 = H[5]*cos(alpha*de_pi)+L[5]*sin(alpha*de_pi);	EA6 = I[5]*cos(alpha*de_pi)+M[5]*sin(alpha*de_pi);
//	EA7 = L[5]*cos(alpha*de_pi)-H[5]*sin(alpha*de_pi);	EA8 = M[5]*cos(alpha*de_pi)-I[5]*sin(alpha*de_pi);
//
//	AA1 = A[0]*F[1] + B[0]*J[1];	AA2 = A[0]*G[1] + B[0]*K[1];	AA3 = B[0]*F[1] - A[0]*J[1];	AA4 = B[0]*G[1] - A[0]*K[1];
//	AB1 = A[0]*H[1] + B[0]*L[1];	AB2 = B[0]*H[1] - A[0]*L[1];
//
//	AD1 = B[0]*(M[1]-dd[1]) + A[0]*(I[1]+aa[1]);	AD2 = B[0]*(I[1]+aa[1]) - A[0]*(M[1]-dd[1]);
//
//	AE1 = F[0]*F[1] + G[0]*J[1];	AE2 = F[0]*G[1] + G[0]*K[1];	AE3 = G[0]*F[1] - F[0]*J[1];	AE4 = G[0]*G[1] - F[0]*K[1];
//	AF1 = F[0]*H[1] + G[0]*L[1];	AF2 = G[0]*H[1] - F[0]*L[1];
//
//	AG1 = G[0]*(M[1]-dd[1]) + F[0]*(I[1]+aa[1]);	AG2 = G[0]*(I[1]+aa[1]) - F[0]*(M[1]-dd[1]);
//
//	AH1 = J[0]*F[1] + K[0]*J[1];	AH2 = K[0]*F[1] - J[0]*J[1];	AH3 = J[0]*G[1] + K[0]*K[1];	AH4 = K[0]*G[1] - J[0]*K[1];
//	AI1 = J[0]*H[1] + K[0]*L[1];	AI2 = K[0]*H[1] - J[0]*L[1];
//	
//	AJ1 = K[0]*(M[1]-dd[1]) + J[0]*(I[1]+aa[1]);	AJ2 = K[0]*(I[1]+aa[1]) - J[0]*(M[1]-dd[1]);
//
//	IA1 = AA1*AD1 + AE1*AG1 + AH1*AJ1;	IA2 = AA2*AD1 + AE2*AG1 + AH3*AJ1;	IA3 = AA3*AD2 + AE3*AG2 + AH2*AJ2;	IA4 = AA4*AD2 + AE4*AG2 + AH4*AJ2;
//	IA5 = AA1*AD2 + AE1*AG2 + AH1*AJ2 + AA3*AD1 + AE3*AG1 + AH2*AJ1;	IA6 = AA2*AD2 + AE2*AG2 + AH3*AJ2 + AA4*AD1 + AE4*AG1 + AH4*AJ1;
//
//	IB1 = D[0]*A[1]*AD1 + AA1*(E[0]+D[0]*E[1]) + H[0]*A[1]*AG1 + AE1*(I[0]+H[0]*E[1]) + L[0]*A[1]*AJ1 + AH1*(M[0]+dd[0]+L[0]*E[1]);
//	IB2 = D[0]*A[1]*AD2 + AA3*(E[0]+D[0]*E[1]) + H[0]*A[1]*AG2 + AE3*(I[0]+H[0]*E[1]) + L[0]*A[1]*AJ2 + AH2*(M[0]+dd[0]+L[0]*E[1]);
//	IB3 = D[0]*B[1]*AD1 + AA2*(E[0]+D[0]*E[1]) + H[0]*B[1]*AG1 + AE2*(I[0]+H[0]*E[1]) + L[0]*B[1]*AJ1 + AH3*(M[0]+dd[0]+L[0]*E[1]);
//	IB4 = D[0]*B[1]*AD2 + AA4*(E[0]+D[0]*E[1]) + H[0]*B[1]*AG2 + AE4*(I[0]+H[0]*E[1]) + L[0]*B[1]*AJ2 + AH4*(M[0]+dd[0]+L[0]*E[1]);
//	IB5 = D[0]*A[1]*(E[0]+D[0]*E[1]) + H[0]*A[1]*(I[0]+H[0]*E[1]) + L[0]*A[1]*(M[0]+dd[0]+L[0]*E[1]);
//	IB6 = D[0]*B[1]*(E[0]+D[0]*E[1]) + H[0]*B[1]*(I[0]+H[0]*E[1]) + L[0]*B[1]*(M[0]+dd[0]+L[0]*E[1]);
//
//	HA1 = AB1*AD1 + AF1*AG1 + AI1*AJ1;	HA2 = AB2*AD2 + AF2*AG2 + AI2*AJ2;
//	HA3 = AB1*AD2 + AB2*AD1 + AF1*AG2 + AF2*AG1 + AI1*AJ2 + AI2*AJ1;
//	HA4 = AB1*(E[0]+D[0]*E[1]) + D[0]*D[1]*AD1 + AF1*(I[0]+H[0]*E[1]) + H[0]*D[1]*AG1 + AI1*(M[0]+dd[0]+L[0]*E[1]) + L[0]*D[1]*AJ1;
//	HA5 = AB2*(E[0]+D[0]*E[1]) + D[0]*D[1]*AD2 + AF2*(I[0]+H[0]*E[1]) + H[0]*D[1]*AG2 + AI2*(M[0]+dd[0]+L[0]*E[1]) + L[0]*D[1]*AJ2;
//	HA6 = D[0]*D[1]*(E[0]+D[0]*E[1]) + H[0]*D[1]*(I[0]+H[0]*E[1]) + L[0]*D[1]*(M[0]+dd[0]+L[0]*E[1]);
//
//	DA1 = A[3]*A[4] - B[3]*BA1;	DA2 = A[3]*B[4] - B[3]*BA2;	DA3 = B[3]*A[4] + A[3]*BA1;	DA4 = B[3]*B[4] + A[3]*BA2;
//
//	DB1 = A[3]*D[4] + B[3]*(H[4]*cos(alpha*de_pi)-L[4]*sin(alpha*de_pi));
//	DB2 = B[3]*D[4] - A[3]*(H[4]*cos(alpha*de_pi)-L[4]*sin(alpha*de_pi));
//	DB3 = -D[3]*(L[4]*cos(alpha*de_pi)+H[4]*sin(alpha*de_pi));
//
//	DD1 = A[3]*(E[4]+aa[4]) + B[3]*(I[4]*cos(alpha*de_pi)-M[4]*sin(alpha*de_pi)-dd[4]);
//	DD2 = B[3]*(E[4]+aa[4]) - A[3]*(I[4]*cos(alpha*de_pi)-M[4]*sin(alpha*de_pi)-dd[4]);
//	DD3 = D[3]*(M[4]*cos(alpha*de_pi)+I[4]*sin(alpha*de_pi)) + (E[3]+aa[3]);
//
//	DE1 = J[3]*A[4] - K[3]*BA1;	DE2 = J[3]*B[4] - K[3]*BA2;	DE3 = K[3]*A[4] + J[3]*BA1;	DE4 = K[3]*B[4] + J[3]*BA2;
//
//	DF1 = J[3]*D[4] + K[3]*(H[4]*cos(alpha*de_pi)-L[4]*sin(alpha*de_pi));
//	DF2 = K[3]*D[4] - J[3]*(H[4]*cos(alpha*de_pi)-L[4]*sin(alpha*de_pi));
//	DF3 = -L[3]*(L[4]*cos(alpha*de_pi)+H[4]*sin(alpha*de_pi));
//
//	DG1 = J[3]*(E[4]+aa[4]) + K[3]*(I[4]*cos(alpha*de_pi)-M[4]*sin(alpha*de_pi)-dd[4]);
//	DG2 = K[3]*(E[4]+aa[4]) - J[3]*(I[4]*cos(alpha*de_pi)-M[4]*sin(alpha*de_pi)-dd[4]);
//	DG3 = L[3]*(M[4]*cos(alpha*de_pi)+I[4]*sin(alpha*de_pi)) + (M[3]+dd[3]);
//
//	DH1 = G[3]*BA1 - F[3]*A[4];	DH2 = G[3]*BA2 - F[3]*B[4];	DH3 = F[3]*BA1 + G[3]*A[4];	DH4 = F[3]*BA2 + G[3]*B[4];
//
//	DI1 = -F[3]*D[4] - G[3]*(H[4]*cos(alpha*de_pi)-L[4]*sin(alpha*de_pi));
//	DI2 = -G[3]*D[4] + F[3]*(H[4]*cos(alpha*de_pi)-L[4]*sin(alpha*de_pi));
//	DI3 = -H[3]*(L[4]*cos(alpha*de_pi)+H[4]*sin(alpha*de_pi));
//
//	DJ1 = -F[3]*(E[4]+aa[4]) - G[3]*(I[4]*cos(alpha*de_pi)-M[4]*sin(alpha*de_pi)-dd[4]);
//	DJ2 = G[3]*(E[4]+aa[4]) - F[3]*(I[4]*cos(alpha*de_pi)-M[4]*sin(alpha*de_pi)-dd[4]);
//	DJ3 = -H[3]*(M[4]*cos(alpha*de_pi)+I[4]*sin(alpha*de_pi)) - I[3];
//
//	FA1 = D[5]*DA1 + EA5*DA2;	FA2 = D[5]*DA2 - EA5*DA1;	FA3 = D[5]*DA3 + EA5*DA4;	FA4 = D[5]*DA4 - EA5*DA3;
//	FA5 = D[3]*D[5]*BA3 + EA5*D[3]*BA4;	FA6 = -D[3]*D[5]*BA4 + EA5*D[3]*BA3;
//
//	FB1 = E[5]*DA1 + EA6*DA2;	FB2 = E[5]*DA2 - EA6*DA1;	FB3 = E[5]*DA3 + EA6*DA4;	FB4 = E[5]*DA4 - EA6*DA3;
//	FB5 = D[3]*E[5]*BA3 + EA6*D[3]*BA4;	FB6 = -D[3]*E[5]*BA4 + EA6*D[3]*BA3;
//	FB7 = EA8*DB1 + DD1;	FB8 = EA8*DB2 + DD2;	FB9 = -EA8*DB3 + DD3;
//
//	FD1 = A[5]*DE1 + EA1*DE2;	FD2 = A[5]*DE2 - EA1*DE1;	FD3 = A[5]*DE3 + EA1*DE4;	FD4 = A[5]*DE4 - EA1*DE3;
//	FD5 = B[5]*DE1 + EA2*DE2;	FD6 = B[5]*DE2 - EA2*DE1;	FD7 = B[5]*DE3 + EA2*DE4;	FD8 = B[5]*DE4 - EA2*DE3;
//
//	FE1 = A[5]*L[3]*BA3 + L[3]*EA1*BA4;	FE2 = -A[5]*L[3]*BA4 + L[3]*EA1*BA3;
//	FE3 = B[5]*L[3]*BA3 + L[3]*EA2*BA4;	FE4 = -B[5]*L[3]*BA4 + L[3]*EA2*BA3;
//
//	FH1 = D[5]*DE1 + EA5*DE2;	FH2 = D[5]*DE2 - EA5*DE1;	FH3 = D[5]*DE3 + EA5*DE4;	FH4 = D[5]*DE4 - EA5*DE3;
//	FH5 = L[3]*D[5]*BA3 + EA5*L[3]*BA4;	FH6 = -L[3]*D[5]*BA4 + EA5*L[3]*BA3;
//
//	FI1 = E[5]*DE1 + EA6*DE2;	FI2 = E[5]*DE2 - EA6*DE1;	FI3 = E[5]*DE3 + EA6*DE4;	FI4 = E[5]*DE4 - EA6*DE3;
//	FI5 = L[3]*E[5]*BA3 + EA6*L[3]*BA4;	FI6 = -L[3]*E[5]*BA4 + EA6*L[3]*BA3;
//	FI7 = EA8*DF1 + DG1;	FI8 = EA8*DF2 + DG2;	FI9 = -EA8*DF3 + DG3;
//
//	FJ1 = D[5]*DH1 + EA5*DH2;	FJ2 = D[5]*DH2 - EA5*DH1;	FJ3 = D[5]*DH3 + EA5*DH4;	FJ4 = -D[5]*DH4 + EA5*DH3;
//	FJ5 = H[3]*D[5]*BA3 + EA5*H[3]*BA4;	FJ6 = H[3]*D[5]*BA4 - EA5*H[3]*BA3;
//
//	FK1 = E[5]*DH1 + EA6*DH2;	FK2 = E[5]*DH2 - EA6*DH1;	FK3 = E[5]*DH3 + EA6*DH4;	FK4 = -E[5]*DH4 + EA6*DH3;
//	FK5 = H[3]*E[5]*BA3 + EA6*H[3]*BA4;	FK6 = H[3]*E[5]*BA4 - EA6*H[3]*BA3;
//	FK7 = EA8*DI1 + DJ1;	FK8 = EA8*DI2 - DJ2;	FK9 = EA8*DI3 + DJ3;
//
//	GA1 = A[2]*AA1 + F[2]*AA2;	GA2 = A[2]*AA2 - F[2]*AA1;	GA3 = A[2]*AA3 + F[2]*AA4;	GA4 = A[2]*AA4 - F[2]*AA3;
//	GA5 = B[2]*AA1 + G[2]*AA2;	GA6 = B[2]*AA2 - G[2]*AA1;	GA7 = B[2]*AA3 + G[2]*AA4;	GA8 = B[2]*AA4 - G[2]*AA3;
//
//	GB1 = A[2]*D[0]*A[1] + F[2]*D[0]*B[1];	GB2 = A[2]*D[0]*B[1] - F[2]*D[0]*A[1];
//	GB3 = B[2]*D[0]*A[1] + G[2]*D[0]*B[1];	GB4 = B[2]*D[0]*B[1] - G[2]*D[0]*A[1];
//
//	GD1 = D[2]*AA1 + H[2]*AA2;	GD2 = D[2]*AA2 - H[2]*AA1;	GD3 = D[2]*AA3 + H[2]*AA4;	GD4 = D[2]*AA4 - H[2]*AA3;
//	GD5 = D[2]*D[0]*A[1] + H[2]*D[0]*B[1];	GD6 = D[2]*D[0]*B[1] - H[2]*D[0]*A[1];
//
//	GE1 = A[2]*AE1 + F[2]*AE2;	GE2 = A[2]*AE2 - F[2]*AE1;	GE3 = A[2]*AE3 + F[2]*AE4;	GE4 = A[2]*AE4 - F[2]*AE3;
//	GE5 = B[2]*AE1 + G[2]*AE2;	GE6 = B[2]*AE2 - G[2]*AE1;	GE7 = B[2]*AE3 + G[2]*AE4;	GE8 = B[2]*AE4 - G[2]*AE3;
//
//	GF1 = A[2]*H[0]*A[1] + F[2]*H[0]*B[1];	GF2 = A[2]*H[0]*B[1] - F[2]*H[0]*A[1];
//	GF3 = B[2]*H[0]*A[1] + G[2]*H[0]*B[1];	GF4 = B[2]*H[0]*B[1] - G[2]*H[0]*A[1];
//
//	GG1 = D[2]*AE1 + H[2]*AE2;	GG2 = D[2]*AE2 - H[2]*AE1;	GG3 = D[2]*AE3 + H[2]*AE4;	GG4 = D[2]*AE4 - H[2]*AE3;
//	GG5 = D[2]*H[0]*A[1] + H[2]*H[0]*B[1];	GG6 = D[2]*H[0]*B[1] - H[2]*H[0]*A[1];
//
//	GH1 = A[2]*AH1 + F[2]*AH3;	GH2 = A[2]*AH2 + F[2]*AH4;	GH3 = A[2]*AH3 - F[2]*AH1;	GH4 = A[2]*AH4 - F[2]*AH2;
//	GH5 = B[2]*AH1 + G[2]*AH3;	GH6 = B[2]*AH2 + G[2]*AH4;	GH7 = B[2]*AH3 - G[2]*AH1;	GH8 = B[2]*AH4 - G[2]*AH2;
//
//	GI1 = A[2]*L[0]*A[1] + F[2]*L[0]*B[1];	GI2 = A[2]*L[0]*B[1] - F[2]*L[0]*A[1];
//	GI3 = B[2]*L[0]*A[1] + G[2]*L[0]*B[1];	GI4 = B[2]*L[0]*B[1] - G[2]*L[0]*A[1];
//
//	GJ1 = D[2]*AH1 + H[2]*AH3;	GJ2 = D[2]*AH2 + H[2]*AH4;	GJ3 = D[2]*AH3 - H[2]*AH1;	GJ4 = D[2]*AH4 - H[2]*AH2;
//	GJ5 = D[2]*L[0]*A[1] + H[2]*L[0]*B[1];	GJ6 = D[2]*L[0]*B[1] - H[2]*L[0]*A[1];
//
//	//입력 받은 TCP 좌표
//	nx = mat[0][0];
//	ny = mat[1][0];
//	nz = mat[2][0];
//	ox = mat[0][1];
//	oy = mat[1][1];
//	oz = mat[2][1];
//	ax = mat[0][2];
//	ay = mat[1][2];
//	az = mat[2][2];
//	px = mat[0][3];
//	py = mat[1][3];
//	pz = mat[2][3];
//
//	//config 값 설정
//	shoulder = config[0];
//	elbow = config[1];
//	wrist = config[2];
//
//	//현재 구하 theta 값이 이전 theta 값과 비교해 차이가 없을 때가지 반복 계산 
//	while(!FindThetaFlag)
//	{
//		iteration++;
//		
//		if(iteration == 1)
//		{
//			//find initial theta 1
//			PP = py;
//			QQ = -px;
//			Init_a5 = Init_d5 = 0;
//			RR = dd[1] - Init_d5*CosTheta[3] + Init_a5*SinTheta[3];
//			PQR = PP*PP+QQ*QQ-RR*RR;
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(PQR < 0)//unreachable
//				return 1;
//
//			CosTheta[0] = (-PP*RR-shoulder*QQ*sqrt(PQR))/(PP*PP+QQ*QQ);
//			SinTheta[0] = (-QQ*RR+shoulder*PP*sqrt(PQR))/(PP*PP+QQ*QQ);
//			theta[0] = atan2(SinTheta[0], CosTheta[0]);
////			sprintf(test, "C[0] = %8.4lf S[0] = %8.4lf T[0] = %8.4lf\n", CosTheta[0], SinTheta[0], theta[0]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			//find initial theta 3
//			JA1 = pz - dd[0];
//			JA2 = px*CosTheta[0] + py*SinTheta[0] - aa[1];
//			JA3 = Init_a5*CosTheta[3] + Init_d5*SinTheta[3] + aa[3];
//
//			PP = 2*aa[2]*JA3;
//			QQ = -2*aa[2]*dd[3];
//			RR = aa[2]*aa[2] + dd[3]*dd[3] - JA1*JA1 - JA2*JA2 + JA3*JA3;
//			PQR = PP*PP+QQ*QQ-RR*RR;
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(PQR < 0)//unreachable
//				return 1;
//
//			CosTheta[2] = (-PP*RR-elbow*QQ*sqrt(PQR))/(PP*PP+QQ*QQ);
//			SinTheta[2] = (-QQ*RR+elbow*PP*sqrt(PQR))/(PP*PP+QQ*QQ);
//			theta[2] = atan2(SinTheta[2], CosTheta[2]);
////			sprintf(test, "C[2] = %8.4lf S[2] = %8.4lf T[2] = %8.4lf\n", CosTheta[2], SinTheta[2], theta[2]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			//find initial theta 2
//			JA4 = JA3*CosTheta[2] - dd[3]*SinTheta[2] + aa[2];
//			JA5 = JA3*SinTheta[2] + dd[3]*CosTheta[2];
//
//			CosTheta[1] = (JA1*JA4+JA2*JA5)/(JA1*JA1+JA2*JA2);
//			SinTheta[1] = (JA2*JA4-JA1*JA5)/(JA1*JA1+JA2*JA2);
//			theta[1] = atan2(SinTheta[1], CosTheta[1]);
////			sprintf(test, "C[1] = %8.4lf S[1] = %8.4lf T[1] = %8.4lf\n", CosTheta[1], SinTheta[1], theta[1]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			//--------------------------------------------------------------------------------------------------
//			nx03
//				= GA1*CosTheta[0]*CosTheta[1]*CosTheta[2] + GA2*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GA3*SinTheta[0]*CosTheta[1]*CosTheta[2] + GA4*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GA5*CosTheta[0]*CosTheta[1]*SinTheta[2] + GA6*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GA7*SinTheta[0]*CosTheta[1]*SinTheta[2] + GA8*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GB1*CosTheta[1]*CosTheta[2] + GB2*SinTheta[1]*CosTheta[2]
//				+ GB3*CosTheta[1]*SinTheta[2] + GB4*SinTheta[1]*SinTheta[2]
//				+ J[2]*AB1*CosTheta[0]*CosTheta[2] + J[2]*AB2*SinTheta[0]*CosTheta[2]
//				+ K[2]*AB1*CosTheta[0]*SinTheta[2] + K[2]*AB2*SinTheta[0]*SinTheta[2]
//				+ J[2]*D[0]*D[1]*CosTheta[2] + K[2]*D[0]*D[1]*SinTheta[2];
//			ny03
//				= GE1*CosTheta[0]*CosTheta[1]*CosTheta[2] + GE2*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GE3*SinTheta[0]*CosTheta[1]*CosTheta[2] + GE4*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GE5*CosTheta[0]*CosTheta[1]*SinTheta[2] + GE6*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GE7*SinTheta[0]*CosTheta[1]*SinTheta[2] + GE8*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GF1*CosTheta[1]*CosTheta[2] + GF2*SinTheta[1]*CosTheta[2]
//				+ GF3*CosTheta[1]*SinTheta[2] + GF4*SinTheta[1]*SinTheta[2]
//				+ J[2]*AF1*CosTheta[0]*CosTheta[2] + J[2]*AF2*SinTheta[0]*CosTheta[2]
//				+ K[2]*AF1*CosTheta[0]*SinTheta[2] + K[2]*AF2*SinTheta[0]*SinTheta[2]
//				+ J[2]*H[0]*D[1]*CosTheta[2] + K[2]*H[0]*D[1]*SinTheta[2];
//			nz03
//				= GH1*CosTheta[0]*CosTheta[1]*CosTheta[2] + GH3*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GH2*SinTheta[0]*CosTheta[1]*CosTheta[2] + GH4*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GH5*CosTheta[0]*CosTheta[1]*SinTheta[2] + GH7*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GH6*SinTheta[0]*CosTheta[1]*SinTheta[2] + GH8*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GI1*CosTheta[1]*CosTheta[2] + GI2*SinTheta[1]*CosTheta[2]
//				+ GI3*CosTheta[1]*SinTheta[2] + GI4*SinTheta[1]*SinTheta[2]
//				+ J[2]*AI1*CosTheta[0]*CosTheta[2] + J[2]*AI2*SinTheta[0]*CosTheta[2]
//				+ K[2]*AI1*CosTheta[0]*SinTheta[2] + K[2]*AI2*SinTheta[0]*SinTheta[2]
//				+ J[2]*L[0]*D[1]*CosTheta[2] + K[2]*L[0]*D[1]*SinTheta[2];
//		
//			ox03
//				= GA5*CosTheta[0]*CosTheta[1]*CosTheta[2] + GA6*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GA7*SinTheta[0]*CosTheta[1]*CosTheta[2] + GA8*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				- GA1*CosTheta[0]*CosTheta[1]*SinTheta[2] - GA2*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				- GA3*SinTheta[0]*CosTheta[1]*SinTheta[2] - GA4*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GB3*CosTheta[1]*CosTheta[2] + GB4*SinTheta[1]*CosTheta[2]
//				- GB1*CosTheta[1]*SinTheta[2] - GB2*SinTheta[1]*SinTheta[2]
//				+ K[2]*AB1*CosTheta[0]*CosTheta[2] + K[2]*AB2*SinTheta[0]*CosTheta[2]
//				- J[2]*AB1*CosTheta[0]*SinTheta[2] - J[2]*AB2*SinTheta[0]*SinTheta[2]
//				+ K[2]*D[0]*D[1]*CosTheta[2] - J[2]*D[0]*D[1]*SinTheta[2];
//			oy03
//				= GE5*CosTheta[0]*CosTheta[1]*CosTheta[2] + GE6*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GE7*SinTheta[0]*CosTheta[1]*CosTheta[2] + GE8*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				- GE1*CosTheta[0]*CosTheta[1]*SinTheta[2] - GE2*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				- GE3*SinTheta[0]*CosTheta[1]*SinTheta[2] - GE4*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GF3*CosTheta[1]*CosTheta[2] + GF4*SinTheta[1]*CosTheta[2]
//				- GF1*CosTheta[1]*SinTheta[2] - GF2*SinTheta[1]*SinTheta[2]
//				+ K[2]*AF1*CosTheta[0]*CosTheta[2] + K[2]*AF2*SinTheta[0]*CosTheta[2]
//				- J[2]*AF1*CosTheta[0]*SinTheta[2] - J[2]*AF2*SinTheta[0]*SinTheta[2]
//				+ K[2]*H[0]*D[1]*CosTheta[2] - J[2]*H[0]*D[1]*SinTheta[2];
//			oz03
//				= GH5*CosTheta[0]*CosTheta[1]*CosTheta[2] + GH7*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GH6*SinTheta[0]*CosTheta[1]*CosTheta[2] + GH8*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				- GH1*CosTheta[0]*CosTheta[1]*SinTheta[2] - GH3*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				- GH2*SinTheta[0]*CosTheta[1]*SinTheta[2] - GH4*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GI3*CosTheta[1]*CosTheta[2] + GI4*SinTheta[1]*CosTheta[2]
//				- GI1*CosTheta[1]*SinTheta[2] - GI2*SinTheta[1]*SinTheta[2]
//				+ K[2]*AI1*CosTheta[0]*CosTheta[2] + K[2]*AI2*SinTheta[0]*CosTheta[2]
//				- J[2]*AI1*CosTheta[0]*SinTheta[2] - J[2]*AI2*SinTheta[0]*SinTheta[2]
//				+ K[2]*L[0]*D[1]*CosTheta[2] - J[2]*L[0]*D[1]*SinTheta[2];
//		
//			ax03
//				= GD1*CosTheta[0]*CosTheta[1] + GD2*CosTheta[0]*SinTheta[1]
//				+ GD3*SinTheta[0]*CosTheta[1] + GD4*SinTheta[0]*SinTheta[1]
//				+ GD5*CosTheta[1] + GD6*SinTheta[1]
//				+ L[2]*AB1*CosTheta[0] + L[2]*AB2*SinTheta[0]
//				+ L[2]*D[0]*D[1];
//			ay03
//				= GG1*CosTheta[0]*CosTheta[1] + GG2*CosTheta[0]*SinTheta[1]
//				+ GG3*SinTheta[0]*CosTheta[1] + GG4*SinTheta[0]*SinTheta[1]
//				+ GG5*CosTheta[1] + GG6*SinTheta[1]
//				+ L[2]*AF1*CosTheta[0] + L[2]*AF2*SinTheta[0]
//				+ L[2]*H[0]*D[1];
//			az03
//				= GJ1*CosTheta[0]*CosTheta[1] + GJ3*CosTheta[0]*SinTheta[1]
//				+ GJ2*SinTheta[0]*CosTheta[1] + GJ4*SinTheta[0]*SinTheta[1]
//				+ GJ5*CosTheta[1] + GJ6*SinTheta[1]
//				+ L[2]*AI1*CosTheta[0] + L[2]*AI2*SinTheta[0]
//				+ L[2]*L[0]*D[1];
//	
//			KA1 = nx03*ax + ny03*ay + nz03*az;
//			KA2 = ox03*nx + oy03*ny + oz03*nz;
//			KA3 = ox03*ox + oy03*oy + oz03*oz;
//			KA4 = ox03*ax + oy03*ay + oz03*az;
//			KA5 = ax03*ax + ay03*ay + az03*az;
//
////			sprintf(test, "nx03 = %8.4lf, ny03 = %8.4lf, nz03 = %8.4lf\nox03 = %8.4lf, oy03 = %8.4lf, oz03 = %8.4lf\nax03 = %8.4lf, ay03 = %8.4lf, az03 = %8.4lf\n", nx03, ny03, nz03, ox03, oy03, oz03, ax03, ay03, az03);
////			sprintf(test, "nx = %8.4lf, ny = %8.4lf, nz = %8.4lf\nox = %8.4lf, oy = %8.4lf, oz = %8.4lf\nax = %8.4lf, ay = %8.4lf, az = %8.4lf\n", nx, ny, nz, ox, oy, oz, ax, ay, az);
//
////			sprintf(test, "KA1 = %8.4lf, KA2 = %8.4lf, KA3 = %8.4lf, KA4 = %8.4lf, KA5 = %8.4lf\n", KA1, KA2, KA3, KA4, KA5);
////			MessageBox(NULL, test, "caution", MB_OK);
//			//----------------------------------------------------------------------------------------------------
//
//			//find initial theta 5
//			CosTheta[4] = (KA4 + pow(cos(alpha*de_pi),2))/(pow(sin(alpha*de_pi),2));
//			PQR = ((KA2*KA2 + KA3*KA3) - pow(sin(alpha*de_pi),2)*pow(cos(alpha*de_pi),2)*pow((CosTheta[4]+1),2))/(pow(sin(alpha*de_pi),2));
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//			if(PQR < 0)//unreachable
//				PQR = 0;
//
//			SinTheta[4] = wrist*sqrt(PQR);
//			theta[4] = atan2(SinTheta[4], CosTheta[4]);
////			sprintf(test, "C[4] = %8.4lf S[4] = %8.4lf T[4] = %8.4lf\n", CosTheta[4], SinTheta[4], theta[4]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//			
//
//			JA1 = sin(alpha*de_pi)*SinTheta[4];
//			JA2 = sin(alpha*de_pi)*cos(alpha*de_pi)*(CosTheta[4]+1);
//			PQR = JA1*JA1+JA2*JA2;
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(PQR < 10e-10)//singular
//				return 2;
//			
//			//find initial theta 4
//			CosTheta[3] = (KA5*JA2-KA1*JA1)/PQR;
//			SinTheta[3] = (KA1*JA2+KA5*JA1)/PQR;
//			theta[3] = atan2(SinTheta[3], CosTheta[3]);
//
//			//find initial theta 6
//			CosTheta[5] = (KA2*JA1+KA3*JA2)/PQR;
//			SinTheta[5] = (KA2*JA2+KA3*JA1)/PQR;
//			theta[5] = atan2(SinTheta[5], CosTheta[5]);
//		}
//		else
//		{
//			//find theta 1
//			pz26
//				= (J[2]*FB1+K[2]*FI1)*CosTheta[2]*CosTheta[3]*CosTheta[4] + (J[2]*FB2+K[2]*FI2)*CosTheta[2]*CosTheta[3]*SinTheta[4]
//				+ (J[2]*FB3+K[2]*FI3)*CosTheta[2]*SinTheta[3]*CosTheta[4] + (J[2]*FB4+K[2]*FI4)*CosTheta[2]*SinTheta[3]*SinTheta[4]
//				+ (K[2]*FB1-J[2]*FI1)*SinTheta[2]*CosTheta[3]*CosTheta[4] + (K[2]*FB2-J[2]*FI2)*SinTheta[2]*CosTheta[3]*SinTheta[4]
//				+ (K[2]*FB3-J[2]*FI3)*SinTheta[2]*SinTheta[3]*CosTheta[4] + (K[2]*FB4-J[2]*FI4)*SinTheta[2]*SinTheta[3]*SinTheta[4]
//				- (J[2]*FB5+K[2]*FI5)*CosTheta[2]*CosTheta[4] + (J[2]*FB6+K[2]*FI6)*CosTheta[2]*SinTheta[4]
//				+ (J[2]*FB7+K[2]*FI7)*CosTheta[2]*CosTheta[3] + (J[2]*FB8+K[2]*FI8)*CosTheta[2]*SinTheta[3]
//				+ (J[2]*FI5-K[2]*FB5)*SinTheta[2]*CosTheta[4] + (K[2]*FB6-J[2]*FI6)*SinTheta[2]*SinTheta[4]
//				+ (K[2]*FB7-J[2]*FI7)*SinTheta[2]*CosTheta[3] + (K[2]*FB8-J[2]*FI8)*SinTheta[2]*SinTheta[3]
//				+ L[2]*FK1*CosTheta[3]*CosTheta[4] + L[2]*FK2*CosTheta[3]*SinTheta[4]
//				- L[2]*FK3*SinTheta[3]*CosTheta[4] + L[2]*FK4*SinTheta[3]*SinTheta[4]
//				+ (J[2]*FB9+K[2]*FI9)*CosTheta[2] + (K[2]*FB9-J[2]*FI9)*SinTheta[2]
//				+ L[2]*FK7*CosTheta[3] + L[2]*FK8*SinTheta[3]
//				+ L[2]*FK5*CosTheta[4] + L[2]*FK6*SinTheta[4]
//				+ L[2]*FK9 + M[2];
//
//			PP = AB1*px + AF1*py + AI1*pz - HA4;
//			QQ = AB2*px + AF2*py + AI2*pz - HA5;
//			RR = D[0]*D[1]*px + H[0]*D[1]*py + L[0]*D[1]*pz - HA6 - HA1*CosTheta[0]*CosTheta[0] - HA2*SinTheta[0]*SinTheta[0] - HA3*SinTheta[0]*CosTheta[0] - pz26;
//			PQR = PP*PP+QQ*QQ-RR*RR;
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(PQR < 0)//unreachable
//				return 1;
//
//			CosTheta[0] = (-PP*RR-shoulder*QQ*sqrt(PQR))/(PP*PP+QQ*QQ);
//			SinTheta[0] = (-QQ*RR+shoulder*PP*sqrt(PQR))/(PP*PP+QQ*QQ);
//			theta[0] = atan2(SinTheta[0], CosTheta[0]);
////			sprintf(test, "C[0] = %lf S[0] = %lf T[0] = %lf\n", CosTheta[0], SinTheta[0], theta[0]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			//find theta 3
//			LA1
//				= (A[2]*FB1+B[2]*FI1)*CosTheta[3]*CosTheta[4] + (A[2]*FB2+B[2]*FI2)*CosTheta[3]*SinTheta[4]
//				+ (A[2]*FB3+B[2]*FI3)*SinTheta[3]*CosTheta[4] + (A[2]*FB4+B[2]*FI4)*SinTheta[3]*SinTheta[4]
//				- (A[2]*FB5+B[2]*FI5)*CosTheta[4] + (A[2]*FB6+B[2]*FI6)*SinTheta[4]
//				+ (A[2]*FB7+B[2]*FI7)*CosTheta[3] + (A[2]*FB8+B[2]*FI8)*SinTheta[3]
//				+ (A[2]*FB9+B[2]*FI9);
//			LA2
//				= (B[2]*FB1-A[2]*FI1)*CosTheta[3]*CosTheta[4] + (B[2]*FB2-A[2]*FI2)*CosTheta[3]*SinTheta[4]
//				+ (B[2]*FB3-A[2]*FI3)*SinTheta[3]*CosTheta[4] + (B[2]*FB4-A[2]*FI4)*SinTheta[3]*SinTheta[4]
//				+ (A[2]*FI5-B[2]*FB5)*CosTheta[4] + (B[2]*FB6-A[2]*FI6)*SinTheta[4]
//				+ (B[2]*FB7-A[2]*FI7)*CosTheta[3] + (B[2]*FB8-A[2]*FI8)*SinTheta[3]
//				+ (B[2]*FB9-A[2]*FI9);
//			LA3
//				= D[2]*FK1*CosTheta[3]*CosTheta[4] + D[2]*FK2*CosTheta[3]*SinTheta[4]
//				- D[2]*FK3*SinTheta[3]*CosTheta[4] + D[2]*FK4*SinTheta[3]*SinTheta[4]
//				+ D[2]*FK7*CosTheta[3] + D[2]*FK8*SinTheta[3]
//				+ D[2]*FK5*CosTheta[4] + D[2]*FK6*SinTheta[4]
//				+ D[2]*FK9 + E[2] + aa[2];
//			LB1
//				= (F[2]*FB1+G[2]*FI1)*CosTheta[3]*CosTheta[4] + (F[2]*FB2+G[2]*FI2)*CosTheta[3]*SinTheta[4]
//				+ (F[2]*FB3+G[2]*FI3)*SinTheta[3]*CosTheta[4] + (F[2]*FB4+G[2]*FI4)*SinTheta[3]*SinTheta[4]
//				- (F[2]*FB5+G[2]*FI5)*CosTheta[4] + (F[2]*FB6+G[2]*FI6)*SinTheta[4]
//				+ (F[2]*FB7+G[2]*FI7)*CosTheta[3] + (F[2]*FB8+G[2]*FI8)*SinTheta[3]
//				+ (F[2]*FB9+G[2]*FI9);
//			LB2
//				= (G[2]*FB1-F[2]*FI1)*CosTheta[3]*CosTheta[4] + (G[2]*FB2-F[2]*FI2)*CosTheta[3]*SinTheta[4]
//				+ (G[2]*FB3-F[2]*FI3)*SinTheta[3]*CosTheta[4] + (G[2]*FB4-F[2]*FI4)*SinTheta[3]*SinTheta[4]
//				+ (F[2]*FI5-G[2]*FB5)*CosTheta[4] + (G[2]*FB6-F[2]*FI6)*SinTheta[4]
//				+ (G[2]*FB7-F[2]*FI7)*CosTheta[3] + (G[2]*FB8-F[2]*FI8)*SinTheta[3]
//				+ (G[2]*FB9-F[2]*FI9);			
//			LB3
//				= H[2]*FK1*CosTheta[3]*CosTheta[4] + H[2]*FK2*CosTheta[3]*SinTheta[4]
//				- H[2]*FK3*SinTheta[3]*CosTheta[4] + H[2]*FK4*SinTheta[3]*SinTheta[4]
//				+ H[2]*FK7*CosTheta[3] + H[2]*FK8*SinTheta[3]
//				+ H[2]*FK5*CosTheta[4] + H[2]*FK6*SinTheta[4]
//				+ H[2]*FK9 + I[2];
//
//			ID1 = IA1*pow(CosTheta[0],2) + IA3*pow(SinTheta[0],2) + IA5*SinTheta[0]*CosTheta[0] + IB1*CosTheta[0] + IB2*SinTheta[0] + IB5;
//			ID2 = IA2*pow(CosTheta[0],2) + IA4*pow(SinTheta[0],2) + IA6*SinTheta[0]*CosTheta[0] + IB3*CosTheta[0] + IB4*SinTheta[0] + IB6;
//
//			JA1 = (AA1*CosTheta[0] + AA3*SinTheta[0] + D[0]*A[1])*px 
//				+ (AE1*CosTheta[0] + AE3*SinTheta[0] + H[0]*A[1])*py 
//				+ (AH1*CosTheta[0] + AH2*SinTheta[0] + L[0]*A[1])*pz - ID1;
//			JA2 = (AA2*CosTheta[0] + AA4*SinTheta[0] + D[0]*B[1])*px 
//				+ (AE2*CosTheta[0] + AE4*SinTheta[0] + H[0]*B[1])*py 
//				+ (AH3*CosTheta[0] + AH4*SinTheta[0] + L[0]*B[1])*pz - ID2;
//			
//			PP = 2*(LA1*LA3+LB1*LB3);
//			QQ = 2*(LA2*LA3+LB2*LB3);
//			RR = (pow(LA1,2)+pow(LB1,2))*pow(CosTheta[2],2)	+ (pow(LA2,2)+pow(LB2,2))*pow(SinTheta[2],2)
//				+ (pow(LA3,2)+pow(LB3,2)) + 2*(LA1*LA2+LB1*LB2)*SinTheta[2]*CosTheta[2] - (JA1*JA1+JA2*JA2);
//			PQR = PP*PP+QQ*QQ-RR*RR;
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(PQR < 0)//unreachable
//				return 1;
//
//			CosTheta[2] = (-PP*RR-elbow*QQ*sqrt(PQR))/(PP*PP+QQ*QQ);
//			SinTheta[2] = (-QQ*RR+elbow*PP*sqrt(PQR))/(PP*PP+QQ*QQ);
//			theta[2] = atan2(SinTheta[2], CosTheta[2]);
////			sprintf(test, "C[2] = %8.4lf S[2] = %8.4lf T[2] = %8.4lf\n", CosTheta[2], SinTheta[2], theta[2]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			//find theta 2
//			JA4 = LA1*CosTheta[2] + LA2*SinTheta[2] + LA3;
//			JA5 = LB1*CosTheta[2] + LB2*SinTheta[2] + LB3;
//
//			CosTheta[1] = (JA1*JA4+JA2*JA5)/(JA1*JA1+JA2*JA2);
//			SinTheta[1] = (JA2*JA4-JA1*JA5)/(JA1*JA1+JA2*JA2);
//			theta[1] = atan2(SinTheta[1], CosTheta[1]);
////			sprintf(test, "C[1] = %8.4lf S[1] = %8.4lf T[1] = %8.4lf\n", CosTheta[1], SinTheta[1], theta[1]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			//--------------------------------------------------------------------------------------------------
//			nx03
//				= GA1*CosTheta[0]*CosTheta[1]*CosTheta[2] + GA2*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GA3*SinTheta[0]*CosTheta[1]*CosTheta[2] + GA4*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GA5*CosTheta[0]*CosTheta[1]*SinTheta[2] + GA6*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GA7*SinTheta[0]*CosTheta[1]*SinTheta[2] + GA8*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GB1*CosTheta[1]*CosTheta[2] + GB2*SinTheta[1]*CosTheta[2]
//				+ GB3*CosTheta[1]*SinTheta[2] + GB4*SinTheta[1]*SinTheta[2]
//				+ J[2]*AB1*CosTheta[0]*CosTheta[2] + J[2]*AB2*SinTheta[0]*CosTheta[2]
//				+ K[2]*AB1*CosTheta[0]*SinTheta[2] + K[2]*AB2*SinTheta[0]*SinTheta[2]
//				+ J[2]*D[0]*D[1]*CosTheta[2] + K[2]*D[0]*D[1]*SinTheta[2];
//			ny03
//				= GE1*CosTheta[0]*CosTheta[1]*CosTheta[2] + GE2*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GE3*SinTheta[0]*CosTheta[1]*CosTheta[2] + GE4*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GE5*CosTheta[0]*CosTheta[1]*SinTheta[2] + GE6*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GE7*SinTheta[0]*CosTheta[1]*SinTheta[2] + GE8*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GF1*CosTheta[1]*CosTheta[2] + GF2*SinTheta[1]*CosTheta[2]
//				+ GF3*CosTheta[1]*SinTheta[2] + GF4*SinTheta[1]*SinTheta[2]
//				+ J[2]*AF1*CosTheta[0]*CosTheta[2] + J[2]*AF2*SinTheta[0]*CosTheta[2]
//				+ K[2]*AF1*CosTheta[0]*SinTheta[2] + K[2]*AF2*SinTheta[0]*SinTheta[2]
//				+ J[2]*H[0]*D[1]*CosTheta[2] + K[2]*H[0]*D[1]*SinTheta[2];
//			nz03
//				= GH1*CosTheta[0]*CosTheta[1]*CosTheta[2] + GH3*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GH2*SinTheta[0]*CosTheta[1]*CosTheta[2] + GH4*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GH5*CosTheta[0]*CosTheta[1]*SinTheta[2] + GH7*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GH6*SinTheta[0]*CosTheta[1]*SinTheta[2] + GH8*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GI1*CosTheta[1]*CosTheta[2] + GI2*SinTheta[1]*CosTheta[2]
//				+ GI3*CosTheta[1]*SinTheta[2] + GI4*SinTheta[1]*SinTheta[2]
//				+ J[2]*AI1*CosTheta[0]*CosTheta[2] + J[2]*AI2*SinTheta[0]*CosTheta[2]
//				+ K[2]*AI1*CosTheta[0]*SinTheta[2] + K[2]*AI2*SinTheta[0]*SinTheta[2]
//				+ J[2]*L[0]*D[1]*CosTheta[2] + K[2]*L[0]*D[1]*SinTheta[2];
//		
//			ox03
//				= GA5*CosTheta[0]*CosTheta[1]*CosTheta[2] + GA6*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GA7*SinTheta[0]*CosTheta[1]*CosTheta[2] + GA8*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				- GA1*CosTheta[0]*CosTheta[1]*SinTheta[2] - GA2*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				- GA3*SinTheta[0]*CosTheta[1]*SinTheta[2] - GA4*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GB3*CosTheta[1]*CosTheta[2] + GB4*SinTheta[1]*CosTheta[2]
//				- GB1*CosTheta[1]*SinTheta[2] - GB2*SinTheta[1]*SinTheta[2]
//				+ K[2]*AB1*CosTheta[0]*CosTheta[2] + K[2]*AB2*SinTheta[0]*CosTheta[2]
//				- J[2]*AB1*CosTheta[0]*SinTheta[2] - J[2]*AB2*SinTheta[0]*SinTheta[2]
//				+ K[2]*D[0]*D[1]*CosTheta[2] - J[2]*D[0]*D[1]*SinTheta[2];
//			oy03
//				= GE5*CosTheta[0]*CosTheta[1]*CosTheta[2] + GE6*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GE7*SinTheta[0]*CosTheta[1]*CosTheta[2] + GE8*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				- GE1*CosTheta[0]*CosTheta[1]*SinTheta[2] - GE2*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				- GE3*SinTheta[0]*CosTheta[1]*SinTheta[2] - GE4*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GF3*CosTheta[1]*CosTheta[2] + GF4*SinTheta[1]*CosTheta[2]
//				- GF1*CosTheta[1]*SinTheta[2] - GF2*SinTheta[1]*SinTheta[2]
//				+ K[2]*AF1*CosTheta[0]*CosTheta[2] + K[2]*AF2*SinTheta[0]*CosTheta[2]
//				- J[2]*AF1*CosTheta[0]*SinTheta[2] - J[2]*AF2*SinTheta[0]*SinTheta[2]
//				+ K[2]*H[0]*D[1]*CosTheta[2] - J[2]*H[0]*D[1]*SinTheta[2];
//			oz03
//				= GH5*CosTheta[0]*CosTheta[1]*CosTheta[2] + GH7*CosTheta[0]*SinTheta[1]*CosTheta[2]
//				+ GH6*SinTheta[0]*CosTheta[1]*CosTheta[2] + GH8*SinTheta[0]*SinTheta[1]*CosTheta[2]
//				- GH1*CosTheta[0]*CosTheta[1]*SinTheta[2] - GH3*CosTheta[0]*SinTheta[1]*SinTheta[2]
//				- GH2*SinTheta[0]*CosTheta[1]*SinTheta[2] - GH4*SinTheta[0]*SinTheta[1]*SinTheta[2]
//				+ GI3*CosTheta[1]*CosTheta[2] + GI4*SinTheta[1]*CosTheta[2]
//				- GI1*CosTheta[1]*SinTheta[2] - GI2*SinTheta[1]*SinTheta[2]
//				+ K[2]*AI1*CosTheta[0]*CosTheta[2] + K[2]*AI2*SinTheta[0]*CosTheta[2]
//				- J[2]*AI1*CosTheta[0]*SinTheta[2] - J[2]*AI2*SinTheta[0]*SinTheta[2]
//				+ K[2]*L[0]*D[1]*CosTheta[2] - J[2]*L[0]*D[1]*SinTheta[2];
//		
//			ax03
//				= GD1*CosTheta[0]*CosTheta[1] + GD2*CosTheta[0]*SinTheta[1]
//				+ GD3*SinTheta[0]*CosTheta[1] + GD4*SinTheta[0]*SinTheta[1]
//				+ GD5*CosTheta[1] + GD6*SinTheta[1]
//				+ L[2]*AB1*CosTheta[0] + L[2]*AB2*SinTheta[0]
//				+ L[2]*D[0]*D[1];
//			ay03
//				= GG1*CosTheta[0]*CosTheta[1] + GG2*CosTheta[0]*SinTheta[1]
//				+ GG3*SinTheta[0]*CosTheta[1] + GG4*SinTheta[0]*SinTheta[1]
//				+ GG5*CosTheta[1] + GG6*SinTheta[1]
//				+ L[2]*AF1*CosTheta[0] + L[2]*AF2*SinTheta[0]
//				+ L[2]*H[0]*D[1];
//			az03
//				= GJ1*CosTheta[0]*CosTheta[1] + GJ3*CosTheta[0]*SinTheta[1]
//				+ GJ2*SinTheta[0]*CosTheta[1] + GJ4*SinTheta[0]*SinTheta[1]
//				+ GJ5*CosTheta[1] + GJ6*SinTheta[1]
//				+ L[2]*AI1*CosTheta[0] + L[2]*AI2*SinTheta[0]
//				+ L[2]*L[0]*D[1];
//	
//			KA1 = nx03*ax + ny03*ay + nz03*az;
//			KA2 = ox03*nx + oy03*ny + oz03*nz;
//			KA3 = ox03*ox + oy03*oy + oz03*oz;
//			KA4 = ox03*ax + oy03*ay + oz03*az;
//			KA5 = ax03*ax + ay03*ay + az03*az;
//			//----------------------------------------------------------------------------------------------------
//
//			//find theta 5
//			//3T6 의 ay를 PQR로 풀었다.
//			PP = FH1*CosTheta[3] + FH3*SinTheta[3] - FH5;
//			QQ = FH2*CosTheta[3] + FH4*SinTheta[3] + FH6;
//			RR = EA7*(DF1*CosTheta[3]+DF2*SinTheta[3]-DF3) - KA4;
//			PQR = PP*PP+QQ*QQ-RR*RR;
////			sprintf(test, "PQR = %lf\n", PQR);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(PQR < 0)//unreachable
//				return 1;
//
//			CosTheta[4] = (-PP*RR-wrist*QQ*sqrt(PQR))/(PP*PP+QQ*QQ);
//			SinTheta[4] = (-QQ*RR+wrist*PP*sqrt(PQR))/(PP*PP+QQ*QQ);
//			theta[4] = atan2(SinTheta[4], CosTheta[4]);
////			sprintf(test, "C[4] = %8.4lf S[4] = %8.4lf T[4] = %lf\n", CosTheta[4], SinTheta[4], theta[4]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
////			sprintf(test, "theta[4] = %lf\n", theta[4]*pi_de);
////			MessageBox(NULL, test, "caution", MB_OK);
//			
//			//find theta 4
//			//3T6 의 ax를 c4에 대해 정리, az를 s4에 대해 정리하여 풀었다.
//			JA1 = FA1*CosTheta[4] + FA2*SinTheta[4] + EA7*DB1;
//			JA2 = FA3*CosTheta[4] + FA4*SinTheta[4] + EA7*DB2;
//			JA3 = -FA5*CosTheta[4] + FA6*SinTheta[4] - EA7*DB3;
////			sprintf(test, "JA1 = %lf\n", JA1);
////			MessageBox(NULL, test, "caution", MB_OK);
//
//			if(fabs(JA1) < 10e-10)//singular
//				return 2;
//			CosTheta[3] = (KA1 - JA2*sin(theta[3]) - JA3)/JA1;
//			
//			JA1 = FJ1*CosTheta[4] + FJ2*SinTheta[4] + EA7*DI1;
//			JA2 = -FJ3*CosTheta[4] + FJ4*SinTheta[4] + EA7*DI2;
//			JA3 = FJ5*CosTheta[4] + FJ6*SinTheta[4] + EA7*DI3;
//			
//			if(fabs(JA2) < 10e-10)//singular
//				return 2;
//			SinTheta[3] = (KA5 - JA1*cos(theta[3]) - JA3)/JA2;
//
//			theta[3] = atan2(SinTheta[3], CosTheta[3]);
//
//			//find theta 6
//			//3T6 의 ny, oy 를 C6, S6 에 대해 정리하여 풀었다.
//			JA1	= FD1*CosTheta[3]*CosTheta[4] + FD2*CosTheta[3]*SinTheta[4] + FD3*SinTheta[3]*CosTheta[4] + FD4*SinTheta[3]*SinTheta[4]
//				- FE1*CosTheta[4] + FE2*SinTheta[4] + DF1*EA3*CosTheta[3] + DF2*EA3*SinTheta[3] - DF3*EA3;
//			JA2	= FD5*CosTheta[3]*CosTheta[4] + FD6*CosTheta[3]*SinTheta[4] + FD7*SinTheta[3]*CosTheta[4] + FD8*SinTheta[3]*SinTheta[4]
//				- FE3*CosTheta[4] + FE4*SinTheta[4] + DF1*EA4*CosTheta[3] + DF2*EA4*SinTheta[3] - DF3*EA4;
//			PQR = JA1*JA1 + JA2*JA2;
//			if(PQR < 10e-10)//singular
//				return 2;
//				
//			CosTheta[5] = (KA2*JA1 + KA3*JA2)/PQR;
//			SinTheta[5] = (KA2*JA2 - KA3*JA1)/PQR;
//			theta[5] = atan2(SinTheta[5], CosTheta[5]);
//		}
//
//		//반복 확인
//		if(iteration == 1)
//		{
//			for(i=0; i<7; i++)
//				thetaPrev[i] = theta[i];
//		}
//		else
//		{
//			for(i=0; i<6; i++)
//				Tolerance[i] = fabs(theta[i] - thetaPrev[i]);
//			
//			if((Tolerance[0] < 10e-10 && Tolerance[1] < 10e-10 && Tolerance[2] < 10e-10 && 
//				Tolerance[3] < 10e-10 && Tolerance[4] < 10e-10 && Tolerance[5] < 10e-10) || iteration >= 100)
//				FindThetaFlag = 1;
//
//			for(i=0; i<7; i++)
//				thetaPrev[i] = theta[i];
//		}
//	}
//
//	for(i=0; i<6; i++)
//		theta[i] = sign[i]*(theta[i]-jointOffset[i]*de_pi);
//
//	return 0;
//}
