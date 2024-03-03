#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2023

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
int		compareDates(const void* pDate1, const void* pDate2);
int		saveDateToBinFile(FILE* file, const Date* pDate);
Date	readDateFromBinFile(FILE* file);
#endif
