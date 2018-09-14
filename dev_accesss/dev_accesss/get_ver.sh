gitUrl="GitUrl:"`git remote get-url --push  origin`
gitLog=`git log -1`
hashNumber=`echo $gitLog | cut -d ' ' -f 2`
hashNumber="GitHashNumber: "$hashNumber
Author=`git log -1|grep ^Auth`
Author="Commit"$Author
commitDate=`git log -1 |grep ^Date`
commitDate="Commit"$commitDate
shortHashNumber=`git rev-parse --short HEAD`

verFile="include/version.h"

cat >${verFile} <<eof
#ifndef _VERSION_H_
#define _VERSION_H_

#define VER_MAJOR 		1
#define VER_MINOR 		0
#define VER_REVISION 	"${shortHashNumber}"

#define GIT_URL 		"${gitUrl}"
#define GIT_HASH_NUM 	"${hashNumber}"
#define GIT_AUTHOR 		"${Author}"
#define COMMIT_DATE 	"${commitDate}"

#endif
eof