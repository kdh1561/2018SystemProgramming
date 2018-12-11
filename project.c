#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void moveFile(char *filename, char *afterDir, char *currentDir);
void copyFile(char *filename, char *afterDir, char *currentDir);
void deleteFile(char *filename);
void compressionFile(char *filename, char *compression);

int main() {

	system("clear");

	char filename[100] = "";
	char afterDir[100] = ""; // 이동 후 디렉토리
	char currentDir[100] = ""; // 현재 디렉토리
	char compression[15] = ""; // 압축방식

	int inputCommand; // 커맨드 입력받을 변수
	int inputCompressionCommend; // 압축 방식 입력받을 변수

	getcwd(currentDir, 100); // 현재 작업중인 디렉토리 위치
	printf("1.movefile\n2.copyfile\n3.deletefile\n4.compression\n>>");
	scanf("%d", &inputCommand);

	switch(inputCommand) {

		case 1:
			strcat(	filename, "test.txt"); //menus[i]
			strcat(afterDir, "/home/kdh/ftpuser/");
			moveFile(filename, afterDir, currentDir);
			break;						
		case 2:
			strcat(filename, "test.txt"); //menus[i]
			strcat(afterDir, "/home/kdh/ftpuser/");
			copyFile(filename, afterDir, currentDir);
			break;
		case 3:
			strcat(filename, "test.txt"); //menus[i]
			strcat(afterDir, "/home/kdh/ftpuser/");
			deleteFile(filename);
			break;
		case 4:
			printf("1.zip   2.tar   3.tar.gz\n>>");
			scanf("%d", &inputCompressionCommend);
			switch(inputCompressionCommend) {
				case 1:
					strcat(compression, "zip");
					break;
				case 2:
					strcat(compression, "tar");
					break;
				case 3:
					strcat(compression, "tar.gz");
					break;
				default:
					printf("잘못된 입력\n");
			}
			strcat(filename, "test.txt"); //menus[i]
			compressionFile(filename, compression);
			break;
		default:
			printf("잘못된 입력\n");
	}


	return 0;
}


// 파일 이동 함수
void moveFile(char *filename, char *afterDir, char *currentDir) {

	char buf[100] = "";
	char space[3] = " ";
	strcat(buf, "mv ");
	strcat(buf, filename);
	strcat(buf, space);
	strcat(buf, afterDir);
	strcat(buf, filename);

	system(buf);

	printf("%s >> %s \n%s : 파일을 이동시켰습니다.\n", currentDir, afterDir, filename);

}

// 파일 복사 함수
void copyFile(char *filename, char *afterDir, char *currentDir) {

	char buf[100] = "";
	char space[3] = " ";
	strcat(buf, "cp ");
	strcat(buf, filename);
	strcat(buf, space);
	strcat(buf, afterDir);
	strcat(buf, filename);

	system(buf);

	printf("%s >> %s \n%s : 파일을 복사했습니다.\n", currentDir, afterDir, filename);

}

// 파일 삭제 함수
void deleteFile(char *filename) {

	char buf[100] = "";
	strcat(buf, "rm -r ");
	strcat(buf, filename);

	system(buf);

	printf("%s : 파일을 삭제했습니다.\n", filename);

}

//파일 압축 함수
void compressionFile(char *filename, char *compression) {

	char buf[100] = "";
	char space[3] = " ";
	char inCompression[15] = "";
	char comp[15] = "";

	// 압축 방식에 따라 함수 내에서 문자열을 따로 만듦
	if( !strcmp(compression, "zip") ) {
		strcat(comp, ".zip");
		strcat(inCompression, "zip ");
	}
	else if( !strcmp(compression, "tar") ) {
		strcat(comp, ".tar");
		strcat(inCompression, "tar -cvf ");
	}
	else if( !strcmp(compression, "tar.gz") ) {
		strcat(comp, ".tar.gz");
		strcat(inCompression, "tar -zcvf ");
	}
	strcat(buf, inCompression);
	strcat(buf, filename);
	strcat(buf, comp);
	strcat(buf, space);
	strcat(buf, filename);

	system(buf);
	sleep(1); // 동작한다는 느낌이 나도록...

	printf(">> %s%s : 파일을 압축했습니다.\n", filename, comp);

}
