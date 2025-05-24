String readStringFromFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading String from file: %s\r\n", path);
  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return ""; //Return an error value
  }
  String label = "";
  while(file.available()){
    label += (char)file.read();
  }
  file.close();
  return label;
}

void writeFile(fs::FS &fs, const char *path, char message[]){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void getting_char_datas_from_flash(){
  readStringFromFile(LittleFS, JSONdatas_file_path).toCharArray(
    serializedJSONdatas,//output
    sizeof(serializedJSONdatas)//size of output
  );
}

void deserialize_JSONdatas(){
  DeserializationError error = deserializeJson(JSONdatas, serializedJSONdatas);
  if (error) {
    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
    //return;
  }
}

void save_datas_to_flash(){
  // Add values in the document
  JSONdatas["sg90_last_consigne_pose"] = sg90_last_consigne_pose;
  JSONdatas["ds3218_last_consigne_pose"] = ds3218_last_consigne_pose;
  // Add an array.
  JsonArray gps_data = JSONdatas["gps_data"].to<JsonArray>();
  gps_data.add(48.756080);
  gps_data.add(2.302038);
  //Serialize datas
  serializeJson(JSONdatas, serializedJSONdatas);
  //Writting to flash
  writeFile(LittleFS, JSONdatas_file_path, serializedJSONdatas);
}

void update_vars_from_JSONdatas(){
  sg90_last_consigne_pose = (uint16_t)JSONdatas["sg90_last_consigne_pose"] ;
  ds3218_last_consigne_pose = (uint16_t)JSONdatas["ds3218_last_consigne_pose"] ;
}