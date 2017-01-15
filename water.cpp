#include <appareo.h>
#include <math.h>
#include <pessum.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "water.h"

using namespace appareo::curse::out;

namespace water {
std::vector<Drink> drinks;
std::vector<Day> days;
int dailygoal = 0, max = 0, units = 1;
int displayformat = false;
pessum::luxreader::DataFile waterlux;
}

void water::MainLoop() {
  appareo::curse::InitializeWindow();
  appareo::curse::windows[0].CreateWindow(
      "Water", -1, appareo::curse::scrheight - 3, 0, 0, true, true);
  BindWindow("Water");
  bool running = true;
  LoadWaterData();
  std::string input = "";
  while (running == true) {
    ReadData();
    appareo::curse::windows[0].Clear();
    if (displayformat == 0) {
      DisplayRecord();
    } else if (displayformat == 1) {
      DisplayDays();
    } else if (displayformat == 2) {
      DisplayGraph();
    }
    appareo::curse::Field newfield;
    newfield.name = "In";
    newfield.type = 4;
    std::vector<appareo::curse::Field> formfields = {newfield};
    formfields = appareo::curse::NewForm(formfields, "Input", -1, 3, 0,
                                         appareo::curse::scrheight - 3);
    input = formfields[0].sval;
    if (input == "q") {
      running = false;
    }
    if (input == "d" || input == "D") {
      displayformat++;
      if (displayformat == 3) {
        displayformat = 0;
      }
    }
    if (input[0] == 'r' || input[0] == 'R') {
      input.erase(input.begin(), input.begin() + 2);
      if (CheckStr(input) == true) {
        Drink newdrink;
        newdrink.daysago = 0;
        newdrink.amount = stoi(input);
        time(&newdrink.date);
        drinks.insert(drinks.begin(), newdrink);
      }
    }
    if (input[0] == 'e' || input[0] == 'E') {
      input.erase(input.begin(), input.begin() + 2);
      if (CheckStr(input) == true) {
        EditDrink(stoi(input) - 1);
      }
    }
  }
  SaveWaterData();
}

void water::DisplayRecord() {
  int width, height, groupsize;
  width = appareo::curse::windows[0].width - 2;
  height = appareo::curse::windows[0].height - 2;
  groupsize = width / 10;
  SetAtt({CYAN_TEXT});
  PrintZ("Daily Goal: " + std::to_string(dailygoal), 2);
  appareo::curse::out::SetAtt();
  std::string labelline;
  labelline = "INDEX";
  while (labelline.size() >= groupsize * 2) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 2) {
    labelline += " ";
  }
  labelline += "AMOUNT";
  while (labelline.size() >= groupsize * 7) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 7) {
    labelline += " ";
  }
  labelline += "DAYS AGO";
  while (labelline.size() >= groupsize * 10) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 10) {
    labelline += " ";
  }
  Print(labelline, 2, 1);
  if (drinks.size() == 0) {
    SetAtt({RED_TEXT, UNDERLINE});
    PrintZ("NO WATER DRUNK", 5);
    SetAtt();
  }
  SetAtt({CYAN_TEXT});
  for (int i = 0; i < drinks.size() && i < height - 6; i++) {
    std::string line = "";
    line += std::to_string(i + 1) + ". ";
    while (line.size() >= groupsize * 2) {
      line.pop_back();
    }
    while (line.size() < groupsize * 2) {
      line += " ";
    }
    line += std::to_string(drinks[i].amount);
    while (line.size() >= groupsize * 7) {
      line.pop_back();
    }
    while (line.size() < groupsize * 7) {
      line += " ";
    }
    line += std::to_string(drinks[i].daysago);
    while (line.size() >= groupsize * 10) {
      line.pop_back();
    }
    while (line.size() < groupsize * 10) {
      line += " ";
    }
    Print(line, i + 3, 1);
  }
  SetAtt();
  PrintZ("Record[r] | Edit[e] | ToggleDisplay[d] | ToggleUnits[u] | Quit[q]",
         7);
}
void water::DisplayDays() {
  int width, height, groupsize;
  width = appareo::curse::windows[0].width - 2;
  height = appareo::curse::windows[0].height - 2;
  groupsize = width / 10;
  SetAtt({CYAN_TEXT});
  PrintZ("Daily Goal: " + std::to_string(dailygoal), 2);
  appareo::curse::out::SetAtt();
  std::string labelline;
  labelline = "DATE";
  while (labelline.size() >= groupsize * 2) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 2) {
    labelline += " ";
  }
  labelline += "TOTAL";
  while (labelline.size() >= groupsize * 4) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 4) {
    labelline += " ";
  }
  labelline += "DRINKS";
  while (labelline.size() >= groupsize * 10) {
    labelline.pop_back();
  }
  while (labelline.size() < groupsize * 10) {
    labelline += " ";
  }
  Print(labelline, 2, 1);
  if (days.size() == 0) {
    SetAtt({RED_TEXT, UNDERLINE});
    PrintZ("NO WATER DRUNK", 5);
    SetAtt();
  }
  for (int i = 0; i < days.size() && i < height - 4; i++) {
    std::string line = "";
    line += appareo::induco::DisplayDate(days[i].date);
    while (line.size() >= groupsize * 2) {
      line.pop_back();
    }
    while (line.size() < groupsize * 2) {
      line += " ";
    }
    line += std::to_string(days[i].total);
    while (line.size() >= groupsize * 4) {
      line.pop_back();
    }
    while (line.size() < groupsize * 4) {
      line += " ";
    }
    for (int j = 0; j < days[i].drinks.size(); j++) {
      line += std::to_string(days[i].drinks[j].amount);
      if (j != days[i].drinks.size() - 1) {
        line += ", ";
      }
    }
    while (line.size() >= groupsize * 10) {
      line.pop_back();
    }
    while (line.size() < groupsize * 10) {
      line += " ";
    }
    if (days[i].total >= dailygoal) {
      SetAtt({CYAN_TEXT});
    } else if (days[i].total >= (dailygoal * 0.9)) {
      SetAtt({BLUE_TEXT});
    } else if (days[i].total >= (dailygoal * 0.8)) {
      SetAtt({GREEN_TEXT});
    } else if (days[i].total >= (dailygoal * 0.6)) {
      SetAtt({YELLOW_TEXT});
    } else {
      SetAtt({RED_TEXT});
    }
    Print(line, i + 3, 1);
    SetAtt();
  }
  PrintZ("Record[r] | Edit[e] | ToggleDisplay[d] | ToggleUnits[u] | Quit[q]",
         7);
}
// UPDATE
void water::DisplayGraph() {
  int width = appareo::curse::windows[0].width - 2;
  int height = appareo::curse::windows[0].height - 5;
  int max = 0;
  std::string linech = " ";
  for(int i = 0; i < days.size(); i++){
    if(days[i].total > max){
      max = days[i].total;
    }
  }
  double worth = max / (double)height;
  for (int i = 0; i < height; i++) {
    if(i * worth > dailygoal && (i - 1) * worth < dailygoal){
      linech = "-";
    }else{
      linech = " ";
    }
    for (int j = 0; j < days.size() && j < width; j++) {
      if(days[j].total > i * worth){
        if (days[j].total >= dailygoal) {
          SetAtt({CYAN_BACK, WHITE_TEXT});
        } else if (days[j].total >= (dailygoal * 0.9)) {
          SetAtt({BLUE_BACK, WHITE_TEXT});
        } else if (days[j].total >= (dailygoal * 0.8)) {
          SetAtt({GREEN_BACK, WHITE_TEXT});
        } else if (days[j].total >= (dailygoal * 0.6)) {
          SetAtt({YELLOW_BACK, WHITE_TEXT});
        } else {
          SetAtt({RED_BACK, WHITE_TEXT});
        }
      }else{
      SetAtt();
    }
      Print(linech, height - i, j + 1);
    }
  }
  SetAtt();
  appareo::curse::windows[0].Update();
  for(int i = 0; i < days.size(); i++){
    pessum::logging::Log(pessum::logging::DATA, std::to_string(i) + ":" + std::to_string(i % 10));
    if(i % 5 == 0){
      std::string day = appareo::induco::DisplayDate(days[i].date, false, false);
      std::string mon = day;
      day.erase(day.begin(), day.end() - 2);
      mon.erase(mon.begin(), mon.begin() + 4);
      mon.erase(mon.end() - 3, mon.end());
      if(day[0] == ' '){
        day.erase(day.begin());
      }
      Print(mon, appareo::curse::windows[0].height - 4, i + 1);
      Print(day, appareo::curse::windows[0].height - 3, i + 1);
    }
  }
  PrintZ("Record[r] | Edit[e] | ToggleDisplay[d] | ToggleUnits[u] | Quit[q]",
         7);
}

void water::LoadWaterData() {
  waterlux = pessum::luxreader::LoadLuxDataFile("/home/arden/bin/files/drinks");
  if (waterlux.datafilevariables[0].stringvalue == "metric") {
    units = 1;
  } else {
    units = 2;
  }
  displayformat = waterlux.datafilevariables[1].intvalue;
  dailygoal = waterlux.datafilevariables[2].intvalue;
  time_t currenttime;
  time(&currenttime);
  struct tm *currenttm;
  currenttm = localtime(&currenttime);
  int currentday = (currenttm->tm_year * 365) + (currenttm->tm_yday);
  for (int i = 0; i < waterlux.datafilevariables[3].intvectorvalues.size();
       i++) {
    Drink newdrink;
    newdrink.amount = waterlux.datafilevariables[3].intvectorvalues[i];
    time_t drinktime = waterlux.datafilevariables[4].intvectorvalues[i];
    currenttm = localtime(&drinktime);
    newdrink.daysago =
        currentday - (currenttm->tm_year * 365 + currenttm->tm_yday);
    // newdrink.daysago = ceil((double)newdrink.daysago / 86400.0);
    newdrink.date = waterlux.datafilevariables[4].intvectorvalues[i];
    drinks.push_back(newdrink);
  }
}

void water::SaveWaterData() {
  if (units == 1) {
    waterlux.datafilevariables[0].stringvalue = "metric";
  }
  waterlux.datafilevariables[1].intvalue = displayformat;
  waterlux.datafilevariables[2].intvalue = dailygoal;
  waterlux.datafilevariables[3].intvectorvalues.clear();
  waterlux.datafilevariables[4].intvectorvalues.clear();
  for (int i = 0; i < drinks.size(); i++) {
    waterlux.datafilevariables[3].intvectorvalues.push_back(drinks[i].amount);
    waterlux.datafilevariables[4].intvectorvalues.push_back(drinks[i].date);
  }
  pessum::luxreader::SaveLuxDataFile("/home/arden/bin/files/drinks", waterlux);
}

void water::EditDrink(int pointer) {
  std::vector<appareo::curse::Field> formfields;
  appareo::curse::Field newfield;
  newfield.name = "Amount";
  newfield.type = 1;
  newfield.sval = std::to_string(drinks[pointer].amount);
  formfields.push_back(newfield);
  newfield.name = "Delete";
  newfield.type = 3;
  newfield.bval = false;
  formfields.push_back(newfield);
  formfields = appareo::curse::NewForm(
      formfields, "Edit Drink #:" + std::to_string(pointer),
      appareo::curse::scrwidth / 2, 10);
  if (formfields[1].bval == true) {
    drinks.erase(drinks.begin() + pointer);
  } else {
    drinks[pointer].amount = formfields[0].ival;
  }
}

void water::ReadData() {
  days.clear();
  int j = 0;
  Day newday;
  time(&newday.date);
  for (int i = 0; i < drinks.size(); i++) {
    if (j < drinks[i].daysago) {
      j++;
      days.push_back(newday);
      newday.drinks.clear();
      newday.date -= 86400;
      newday.total = 0;
    }
    newday.drinks.push_back(drinks[i]);
    newday.total += drinks[i].amount;
  }
  days.push_back(newday);
}

bool water::CheckStr(std::string str) {
  for (int i = 0; i < str.size(); i++) {
    if (int(str[i]) < 48 || int(str[i]) > 57) {
      return (false);
    }
  }
  return (true);
}
