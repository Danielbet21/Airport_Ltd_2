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

//int compareByDay(const void* flight1, const void* flight2);
//int compareByMonth(const void* flight1, const void* flight2);
//int compareByYear(const void* flight1, const void* flight2);
int compareDates(const void* pDate1, const void* pDate2);
#endif
