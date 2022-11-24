// 추가된 부분 : 매직 넘버와 명령어 번호
#define LCD_DEV_MAGIC 'Y'
#define LCD_INIT _IO(LCD_DEV_MAGIC, 0)
#define LCD_SET_POS _IOW(LCD_DEV_MAGIC, 1, int)
//
