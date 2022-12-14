using namespace std;

string getDate(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "Failed to obtain time";
   }
//  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  uint8_t day = timeinfo.tm_mday;
  string dayStr = day < 10 ? "0" + to_string(day) : to_string(day);
   
  uint8_t month = timeinfo.tm_mon + 1;
  string monthStr = month < 10 ? "0" + to_string(month) : to_string(month);
  
  uint16_t year = timeinfo.tm_year + 1900;
  string yearStr = to_string(year);

  return dayStr + "." + monthStr + "." + yearStr;
}

string getTime(int* time_arr) {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "Failed to obtain time";
  }
  string hour_str = to_string(timeinfo.tm_hour);
  time_arr[0]=stringToNum(hour_str);
  string minute_str = to_string(timeinfo.tm_min);
  time_arr[1]=stringToNum(minute_str);
  string sec_str = to_string(timeinfo.tm_sec);
  time_arr[2]=stringToNum(sec_str);
  
  return hour_str + ":" + minute_str + ":" + sec_str;
  
}