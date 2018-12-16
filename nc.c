#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define size 100
#define TRUE 1
#define FALSE 0

void check_FileInfo(char *name, int type, char *info);
void UI();
void FileManagementUI(char *filename, char *info, int type, int nitem, int flag);
int open_dir(char (*menu)[30], char (*select_menu)[30], char path[5], int *type);
void ftpfiletransfer(char *filename);

int main(int argc, char *argv[])
{
	UI();
}

void UI()
{
	int max_x = 0, max_y = 0;
	int ch;
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	getmaxyx(stdscr, max_y, max_x);

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);

	int currItem = 0;
	int cont = 1;
	int flag = 1;
	char menu[size][30] = {0,};
	char select_menu[size][30] = {0,};
	char info[40] = {0,};
	int type[size] = {0,};
	int nitem = open_dir(menu, select_menu, ".", type);
	while(cont)
	{

		char temp[100] = "";
		clear();
		for(int i = 0; i < nitem; i++)
		{
			if(currItem == i)
			{
				switch(type[i])
				{
					case DT_REG : 
						attron(COLOR_PAIR(1));
						break;
					case DT_DIR :
						attron(COLOR_PAIR(2));
						break;
					default :
						attron(COLOR_PAIR(3));
						break;
				}
				mvprintw(i, 0, select_menu[i]);
				attron(COLOR_PAIR(3));
			}
			else
				mvprintw(i, 0, menu[i]);
		}
		attron(COLOR_PAIR(3));
		if(strcmp(menu[currItem], ".") && strcmp(menu[currItem], ".."))
			check_FileInfo(menu[currItem], type[currItem], info);
		FileManagementUI(menu[currItem], info, type[currItem], nitem, flag);

		ch = (int) getch();
		switch(ch)
		{
			case KEY_UP :
				if(currItem > 0) currItem --;
				break;
			case KEY_DOWN :
				if(currItem + 1 < nitem) currItem ++;
				break;
			case KEY_RIGHT :
				if(strcmp(menu[currItem], "..") || strcmp(menu[currItem], "."))
					flag = 0;
				break;
			case KEY_LEFT :
				if(strcmp(menu[currItem], "..") || strcmp(menu[currItem], "."))
					flag = 1;
				break;
			case 'q' :
				cont = 0;
				break;
			case '\n' :
				if(!strcmp(menu[currItem], ".."))
				{
					nitem = open_dir(menu, select_menu, "..", type);
					chdir("..");
				}
				else if(type[currItem] == DT_DIR)
				{
					getcwd(temp, 100);
					strcpy(temp, "/");
					strcpy(temp, menu[currItem]);
					chdir(temp);
					nitem = open_dir(menu, select_menu, ".", type);
				}
				currItem = 0;
				break;
			case '1' :
				if(flag == 0)
				{
					mvprintw(18, 30, "* INPUT MOVING ADDRESS WHICH YOU WANT (%s)", menu[currItem]);
					int d = getch();
					int  count = 0;
					while(d != '\n')
					{
						temp[count] = d;
						mvprintw(16, 30+(count++), "%c", d);
					}
					temp[count] = '\0';
				}
				break;
			case '2' :
				if(flag == 0)
				{
					mvprintw(19, 30, "* INPUT COPYING ADDRESS WHICH YOU WANT (%s)", menu[currItem]);
				}
				break;
			case '3' :
				
				if(flag == 0)
				{
					//system("rm -r AA.txt");
					strcat(temp, "rm -r ");
					strcat(temp, menu[currItem]);
					system(temp);
					nitem = open_dir(menu, select_menu, ".", type);
				}
				break;
			case 4 :
				if(flag)
				{

				}
				break;
			case '5' :
				if(flag == 0)
				{
					strcat(temp, menu[currItem]);
					ftpfiletransfer(temp);	
					break;
				}
				break;
			default :
				break;

		}
		refresh();
	}
	endwin();
	return ;
}
void FileManagementUI(char *filename, char *info, int type, int nitem, int flag)
{
	char *menu[20] = {"PRESS KEY-RIGHT TO MANAGE FILE"," - 1. MOVE", " - 2. COPY", " - 3. DELETE", " - 4. COMPRESS", " - 5. TRANSFER", " - q: EXIT"};
	int ch;
	mvhline(0, 30, '*', 50);
	for(int i = 0; i < 7; i++)
	{
		mvprintw(i+1, 30, "*");
		mvprintw(i+1, 30 + 2, "%s", menu[i]);
		mvprintw(i+1, 79, "*");
	}
	mvprintw(8, 30, "*");
	mvprintw(8, 79, "*");
	mvhline(9, 30, '*', 50);
	mvprintw(10, 30, "*");
	mvprintw(10, 79, "*");
	mvprintw(11, 79, "*");
	mvprintw(12, 30, "*");
	mvprintw(12, 79, "*");
	mvhline(13, 30, '*', 50);

	if(!strncmp(filename, "..", 2))
		mvprintw(11, 30, "* GO TO PREVIOUS DIRECTORY");
	else if(!strncmp(filename, ".", 1))
		mvprintw(11, 30, "* STAY ON CURRENT DIRECTORY");
	else
		if(flag)
			mvprintw(11, 30, "* %s", info);
		else
			mvprintw(11, 30, "* PRESS SOMETHING ");

}
void check_FileInfo(char *name, int type, char *info)
{
	struct stat fileinfo;
	char mode[12];
	stat(name, &fileinfo);
	memset(info, '\0', 40);
	switch(type)
	{
		case DT_DIR :
			strcat(info, "DIRECTORY | ");
			break;
		case DT_REG :
			strcat(info, "FILE | ");
			break;
		default :
			strcat(info, "UNKNOWN | ");
	}
	sprintf(mode, "%ld | ", fileinfo.st_size);
	strcat(info, " SIZE : ");
	strcat(info, mode);

	mode[0] = (fileinfo.st_mode & S_IRUSR) ? 'R' : '-';
	mode[1] = (fileinfo.st_mode & S_IWUSR) ? 'W' : '-';
	mode[2] = (fileinfo.st_mode & S_IXUSR) ? 'X' : '-';
	mode[3] = ' ';

	mode[4] = (fileinfo.st_mode & S_IRGRP) ? 'R' : '-';
	mode[5] = (fileinfo.st_mode & S_IWGRP) ? 'W' : '-';
	mode[6] = (fileinfo.st_mode & S_IXGRP) ? 'X' : '-';
	mode[7] = ' ';

	mode[8] = (fileinfo.st_mode & S_IROTH) ? 'R' : '-';
	mode[9] = (fileinfo.st_mode & S_IWOTH) ? 'W' : '-';
	mode[10] = (fileinfo.st_mode & S_IXOTH) ? 'X' : '-';
	mode[11] = '\0';

	strcat(info, mode);
}


int open_dir(char (*menu)[30], char (*select_menu)[30], char path[5], int *type)
{
	struct dirent **entry;
	int nitem = 0;
	int count;
	DIR *dir;

	if((count = scandir(path, &entry, NULL, alphasort)) == -1) return -1;
	for(nitem = 0; nitem < count; nitem++)
	{
		memset(menu[nitem], '\0', 30);
		memset(select_menu[nitem], '\0', 30);;
		type[nitem] = 0;
		strcat(menu[nitem], entry[nitem]->d_name);
		strcat(select_menu[nitem], "> ");
		strcat(select_menu[nitem], entry[nitem]->d_name);
		type[nitem] = entry[nitem]->d_type;
		free(entry[nitem]);
	}
	free(entry);
	return nitem;
}



//파일 압축 함수
void ftpfiletransfer(char *filename) {

	char buf[100] = "";
	char space[3] = " ";
	char comp[15] = ".zip";
	char fn[100] = "ftpfile";
	char compression[10] = "zip ";
	char enter5[20] = "\n\n\n\n\n";

	//int isUser = access( "/home/ftpuser/", 0 );

	// ftp서버를 열어주는 부분.
	// sudo /etc/init.d/vsftpd start와
	// sudo adduser ftpuser 부분 직접 세팅해줘야합니다.
	// 123
	// 123
	/*
	system("sudo /etc/init.d/vsftpd start");
	*/
	//만약 해당 유저가 없으면 유저를 생성해줌.
	//이건 GUI로 구현했을 대 문제의 소지가 많기에
	//기존에 미리 생성해두는 것이 좋아보입니다.	
	/*
	if( isUser == -1 ) {
		system("sudo adduser ftpuser");
		system("sudo chmod 757 /home/ftpuser");
	}*/
	
	//이미 압축파일이면 파일 이름을 변경해서 보내고
	if(strstr(filename, "zip")) {
		strcat(buf, "cp ");
		strcat(buf, filename);
		strcat(buf, " /home/ftpuser/ftpfile.zip");
		system(buf);
	}
	else { // 압축파일이 아니면 압축해서 보냄
		strcat(buf, compression);
		strcat(buf, fn);
		strcat(buf, comp);
		strcat(buf, space);
		strcat(buf, filename);
		system(buf);
		system("mv ftpfile.zip /home/ftpuser/");
	}

	//어차피 서버를 열어줄 때 sudo를 사용했음.
	system("sudo chmod 777 /home/ftpuser/ftpfile.zip");

	printf(">> FTP 전송 준비 완료\n");

}
