void showTime()
{
  Serial.print(ts.year);Serial.print("/");Serial.print(ts.month);Serial.print("/");Serial.println(ts.day);
  Serial.print(ts.hour);Serial.print(":");Serial.print(ts.minute);Serial.print(":");Serial.println(ts.second);
  Serial.print(":");Serial.println(getDayOfWeek(ts.year, ts.month, ts.day));
}
