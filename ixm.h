struct ixm_service{
	int mode;
	int func;
	double param;
};

typedef struct ixm_service *ixm_t;

/////MODE/////
#define IXM_MODE_WATCH 1
#define IXM_MODE_KEYOP 2

/////FUNC/////
#define IXM_FUNC_FOLLOW 1
#define IXM_FUNC_FALL 2
#define IXM_FUNC_TEMPERTURE 3
#define IXM_FUNC_FOLLOW_AND_FALL 4
#define IXM_FUNC_FOLLOW_AND_TEMPERTURE 5
#define IXM_FUNC_FALL_AND_TEMPERTURE 6
#define IXM_FUNC_FOLLOW_AND_FALL_AND_TEMPERTURE 7
#define IXM_FUNC_KEYOP 1

int StartIxm();

int IxmSetMode(struct ixm_service *ixm,int mode);
int IxmSetFunc(struct ixm_service *ixm,int func);
int IxmSetParam(struct ixm_service *ixm,float param);
int IxmStart(struct ixm_service *ixm);
int IxmGetMode(struct ixm_service *ixm);
int IxmGetFunc(struct ixm_service *ixm);
int IxmGetParam(struct ixm_service *ixm);

void KobukiModeSet(struct ixm_service *ixm);
void KobukiDistanceSet(struct ixm_service *ixm);
void IxmStop();
void roslaunch();
void KobukiStart();
void follow();
void fall();
void temperture();
void keyop();

int child_process_id[7];
int stop_count = 0;
int loop_count = 0;
