#ifndef WATER_H_
#define WATER_H_
#include <pessum.h>
#include <appareo.h>
#include <strings.h>
#include <vector>
namespace water {
struct Drink {
  int amount = 0, daysago = 0;
  time_t date;
};
struct Day {
  time_t date;
  std::vector<Drink> drinks;
  int total = 0;
};
extern std::vector<Drink> drinks;
extern std::vector<Day> days;
extern int dailygoal, max, units;
extern int displayformat;
extern pessum::luxreader::DataFile waterlux;

void MainLoop();
void DisplayRecord();
void DisplayDays();
void DisplayGraph();
void LoadWaterData();
void SaveWaterData();
void EditDrink(int pointer);
void ReadData();
bool CheckStr(std::string str);
}
#endif
