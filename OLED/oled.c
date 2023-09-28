#include "oled.h"

#define OLED_ARDESS 0x78

#define OLED_CMD 0x00  /* 命令 */
#define OLED_DATA 0x40 /* 数据 */

uint8_t OLED_RAM[128][4];
// 16*16 ASCII字符集点阵
const unsigned char asc2_1608[][16] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*" ",0*/
	{0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00}, /*"!",1*/
	{0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*""",2*/
	{0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00}, /*"#",3*/
	{0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00}, /*"$",4*/
	{0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00}, /*"%",5*/
	{0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10}, /*"&",6*/
	{0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"'",7*/
	{0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00}, /*"(",8*/
	{0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00}, /*")",9*/
	{0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00}, /*"*",10*/
	{0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00}, /*"+",11*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00}, /*",",12*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, /*"-",13*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00}, /*".",14*/
	{0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00}, /*"/",15*/
	{0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00}, /*"0",16*/
	{0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"1",17*/
	{0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00}, /*"2",18*/
	{0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"3",19*/
	{0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00}, /*"4",20*/
	{0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"5",21*/
	{0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"6",22*/
	{0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00}, /*"7",23*/
	{0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00}, /*"8",24*/
	{0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00}, /*"9",25*/
	{0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00}, /*":",26*/
	{0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00}, /*";",27*/
	{0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00}, /*"<",28*/
	{0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00}, /*"=",29*/
	{0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00}, /*">",30*/
	{0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x36, 0x01, 0x00, 0x00}, /*"?",31*/
	{0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00, 0x07, 0x18, 0x27, 0x24, 0x23, 0x14, 0x0B, 0x00}, /*"@",32*/
	{0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x23, 0x02, 0x02, 0x27, 0x38, 0x20}, /*"A",33*/
	{0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"B",34*/
	{0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00}, /*"C",35*/
	{0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00}, /*"D",36*/
	{0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00}, /*"E",37*/
	{0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00}, /*"F",38*/
	{0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00}, /*"G",39*/
	{0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20}, /*"H",40*/
	{0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"I",41*/
	{0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, 0x00}, /*"J",42*/
	{0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00, 0x20, 0x3F, 0x20, 0x01, 0x26, 0x38, 0x20, 0x00}, /*"K",43*/
	{0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00}, /*"L",44*/
	{0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00, 0x20, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x20, 0x00}, /*"M",45*/
	{0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x20, 0x00, 0x07, 0x18, 0x3F, 0x00}, /*"N",46*/
	{0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00}, /*"O",47*/
	{0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x01, 0x00, 0x00}, /*"P",48*/
	{0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x18, 0x24, 0x24, 0x38, 0x50, 0x4F, 0x00}, /*"Q",49*/
	{0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20}, /*"R",50*/
	{0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00}, /*"S",51*/
	{0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00}, /*"T",52*/
	{0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00}, /*"U",53*/
	{0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x07, 0x38, 0x0E, 0x01, 0x00, 0x00}, /*"V",54*/
	{0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00, 0x03, 0x3C, 0x07, 0x00, 0x07, 0x3C, 0x03, 0x00}, /*"W",55*/
	{0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08, 0x20, 0x30, 0x2C, 0x03, 0x03, 0x2C, 0x30, 0x20}, /*"X",56*/
	{0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00}, /*"Y",57*/
	{0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x20, 0x38, 0x26, 0x21, 0x20, 0x20, 0x18, 0x00}, /*"Z",58*/
	{0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00}, /*"[",59*/
	{0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x38, 0xC0, 0x00}, /*"\",60*/
	{0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00}, /*"]",61*/
	{0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"^",62*/
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, /*"_",63*/
	{0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"`",64*/
	{0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x19, 0x24, 0x22, 0x22, 0x22, 0x3F, 0x20}, /*"a",65*/
	{0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"b",66*/
	{0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x20, 0x11, 0x00}, /*"c",67*/
	{0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x10, 0x3F, 0x20}, /*"d",68*/
	{0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x22, 0x13, 0x00}, /*"e",69*/
	{0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"f",70*/
	{0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6B, 0x94, 0x94, 0x94, 0x93, 0x60, 0x00}, /*"g",71*/
	{0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20}, /*"h",72*/
	{0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"i",73*/
	{0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00}, /*"j",74*/
	{0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x24, 0x02, 0x2D, 0x30, 0x20, 0x00}, /*"k",75*/
	{0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00}, /*"l",76*/
	{0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x3F, 0x20, 0x00, 0x3F}, /*"m",77*/
	{0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20}, /*"n",78*/
	{0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00}, /*"o",79*/
	{0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xA1, 0x20, 0x20, 0x11, 0x0E, 0x00}, /*"p",80*/
	{0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0xA0, 0xFF, 0x80}, /*"q",81*/
	{0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x3F, 0x21, 0x20, 0x00, 0x01, 0x00}, /*"r",82*/
	{0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x33, 0x24, 0x24, 0x24, 0x24, 0x19, 0x00}, /*"s",83*/
	{0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x00, 0x00}, /*"t",84*/
	{0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x10, 0x3F, 0x20}, /*"u",85*/
	{0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x01, 0x0E, 0x30, 0x08, 0x06, 0x01, 0x00}, /*"v",86*/
	{0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x0F, 0x30, 0x0C, 0x03, 0x0C, 0x30, 0x0F, 0x00}, /*"w",87*/
	{0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x31, 0x2E, 0x0E, 0x31, 0x20, 0x00}, /*"x",88*/
	{0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x8E, 0x70, 0x18, 0x06, 0x01, 0x00}, /*"y",89*/
	{0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x21, 0x30, 0x2C, 0x22, 0x21, 0x30, 0x00}, /*"z",90*/
	{0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x40, 0x40}, /*"{",91*/
	{0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00}, /*"|",92*/
	{0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00}, /*"}",93*/
	{0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"~",94*/
};
/**
 * 函数功能: OLED发送一个字节
 * 输入参数: dat	发送的数据
 * 输入参数: mode:数据/命令标志 OLED_CMD 表示命令 OLED_DATA 表示数据;
 * 说    明:	实际是发送三个字节 分别位  器件地址  命令字符  数据
 */
void OLED_Write_Byte(uint8_t dat, uint8_t mode)
{
	iic_start();
	iic_send_byte(OLED_ARDESS); /* 写地址 */
	iic_wait_ack();
	iic_send_byte(mode); /* 写命令字符 */
	iic_wait_ack();
	iic_send_byte(dat); /* 写数据*/
	iic_wait_ack();
	iic_stop();
}

/**
 * 函数功能: OLED初始化
 * 说    明:	没有包含IIC引脚的初始化所以要放在IIC初始化之后
 */
void OLED_Init(void)
{

	HAL_Delay(100);					 /* OLED上电比MCU慢 */
	OLED_Write_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_Write_Byte(0x00, OLED_CMD); /*set lower column address*/
	OLED_Write_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_Write_Byte(0x00, OLED_CMD); /*set display start line*/
	OLED_Write_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_Write_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_Write_Byte(0xff, OLED_CMD); /*128*/
	OLED_Write_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_Write_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_Write_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_Write_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
	OLED_Write_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_Write_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_Write_Byte(0x80, OLED_CMD);
	OLED_Write_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_Write_Byte(0x1f, OLED_CMD);
	OLED_Write_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xdb, OLED_CMD); /*set vcomh*/
	OLED_Write_Byte(0x40, OLED_CMD);
	OLED_Write_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
	OLED_Write_Byte(0x14, OLED_CMD);
	OLED_Clear(0);
	OLED_Write_Byte(0xAF, OLED_CMD); /*display ON*/
}

/**
 * 函数功能: 开启OLED显示
 * 说    明: 无
 */
void OLED_DisPlay_On(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD); /* 电荷泵使能 */
	OLED_Write_Byte(0x14, OLED_CMD); /* 开启电荷泵 */
	OLED_Write_Byte(0xAF, OLED_CMD); /* 点亮屏幕 */
}

/**
 * 函数功能: 关闭OLED显示
 * 说    明: 无
 */
void OLED_DisPlay_Off(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD); /* 电荷泵使能 */
	OLED_Write_Byte(0x10, OLED_CMD); /* 关闭电荷泵 */
	OLED_Write_Byte(0xAE, OLED_CMD); /* 关闭屏幕 */
}

/**
 * 函数功能: OLED刷屏实际写数据函数
 * 输入参数: 无
 * 输入参数: 无
 * 说    明:	无
 */
void OLED_Refresh(void)
{
	uint8_t i, n;
	for (i = 0; i < 4; i++)
	{
		OLED_Write_Byte(0xb0 + i, OLED_CMD); /* 设置行起始地址 */
		OLED_Write_Byte(0x00, OLED_CMD);	 /* 设置低列起始地址 只取0x00的低四位 */
		OLED_Write_Byte(0x10, OLED_CMD);	 /* 设置高列起始地址 只取0x10的低四位*/
		iic_start();						 /* 合计 0x00 所以起始列地址 0x00 */
		iic_send_byte(0x78);
		iic_wait_ack();
		iic_send_byte(0x40);
		iic_wait_ack();
		for (n = 0; n < 128; n++)
		{
			iic_send_byte(OLED_RAM[n][i]);
			iic_wait_ack();
		}
		iic_stop();
	}
}

/**
 * 函数功能: OLED屏幕清屏函数
 * 输入参数: mode 全屏点亮刷屏/全屏熄灭刷屏  1:亮  0:灭
 * 说    明:	无
 */
void OLED_Clear(uint8_t mode)
{
	uint8_t dat;
	uint8_t i, j;

	if (mode)
	{
		dat = 0xff;
	}
	else
	{
		dat = 0x00;
	}
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 128; j++)
		{
			OLED_RAM[j][i] = dat;
		}
	}
	OLED_Refresh(); /* 更新显示 */
}

/**
 * 函数功能: 画点函数
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: t 填充/清空 1:填充 0:清空
 * 说    明:	无
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
	uint8_t i, j;
	i = y / 8; /* 算出数据在第几行 */
	j = y % 8; /* 算出数据在第几行的具体哪一位*/
	if (t)
	{
		OLED_RAM[x][i] |= 1 << j;
	}
	else
	{
		OLED_RAM[x][i] &= ~(1 << j);
	}
}

/**
 * 函数功能: 显示一个英文字符
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: chr 需要显示的字符
 * 输入参数: char_size 显示字符的大小
 * 输入参数: mode 正常显示/反色显示 1 正常显示 0 反色显示
 * 说    明: 兼容 8*6的字体
 */
/*
1. ----->		2. ------->
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
3. ----->		4. ------->
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
*/
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode)
{
	uint8_t ram_size, size1, chr1, array, x0, y0;
	x0 = x, y0 = y;

	ram_size = char_size / 8;
	if (char_size % 8)
	{
		ram_size += 1;
	}
	ram_size = ram_size * (char_size / 2); /* 这个/2就是得出这个字体的宽度是多少 如12*6 32*16, 如果要改成长和宽一样的字体就要改成 /1 同理推出其他的字体 */

	if(char_size == 8) ram_size = 6;	/* 为了兼容 8*6 的字体 */

	chr1 = chr - ' '; /* 计算偏移后的值 */

	for (uint8_t i = 0; i < ram_size; i++)
	{
		switch (char_size)
		{
			case(8) :break;
			case(16): array = asc2_1608[chr1][i]; break;
			case(24):break;
			case(32):break;
		}

		for (uint8_t j = 0; j < 8; j++) /* 写入一个字节的数据*/
		{
			if (array & 0x01) /* 按位读取字节数据*/
			{
				OLED_DrawPoint(x, y, mode);
			}
			else
			{
				OLED_DrawPoint(x, y, !mode);
			}
			array = array >> 1; /* 移位操作 */
			y += 1;				/* 显示的y轴位置加一 */
		}

		x += 1;										 /* x轴向右加一 */
		if ((size1 != 8) && ((x - x0) == size1 / 2)) /* 如果这一行数据显示完了,开始写入下一行的数据 */
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0; /* y的值加8*/
	}
}

/**
 * 函数功能: 显示英文字符串
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: str 需要显示的字符串
 * 输入参数: char_size 显示字符串的大小
 * 输入参数: mode 正常显示/反色显示 1 正常显示 0 反色显示
 * 说    明: 兼容 8*6的字体
 */
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str, uint8_t str_size, uint8_t mode)
{
	while ((*str >= ' ') && (*str <= '~'))
	{
		OLED_Show_Char(x, y, *str, str_size, mode);
		x += str_size / 2;
		str++;
	}
}
