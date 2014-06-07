#ifndef _UTIL_MAV_H_
#define _UTIL_MAV_H_

/**************************************************************************
FUNCTION PROTOTYPES
**************************************************************************/

extern int  getVoteMaxBytes(int , int );
extern int  getVoteMaxVotes(int , int );
extern void addVoteMark(char *, int);
extern int  isVoteMark(char *);
extern int  existScrutin(int );

#endif /* _UTIL_MAV_H_ */
