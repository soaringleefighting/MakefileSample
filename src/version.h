#ifndef VERSION_H_  
#define VERSION_H_  
//major.minor.svn(git)version.datetime_base   
#define MAJOR_VERSION   ("0")  
#define MINOR_VERSION	("1")  
#if _WIN32  
#define SG_BRANCH_NAME	("*master")  
#define SG_COMMIT_HASH	("3345ca3fbc4525255107a6e4b9752fb071a2b8db")  
#define SG_COMMIT_DATE	("2020-08-1611:20:20+0800")  
#define SG_VERSION		(SG_COMMIT_HASH)  
#else  
#ifdef ANDROID 
#define SG_VERSION		(1000) //TODO: support git version for android platform 
#else 
#define SG_VERSION		(GIT_VERSION)  
#endif  
#endif /* #if _WIN32 */  
#endif /* #ifndef VERSION_H_ */ 
