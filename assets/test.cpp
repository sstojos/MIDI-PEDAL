class AsyncHTTPSRequest {

  public: 
    int testVar = 0;
    char request[100] = "";
    volatile bool taskFinished = true;
  private: 
    const int taskCore = 0;
    const int taskPriority = 1;
    TaskHandle_t Task1;
  
  public:  AsyncHTTPSRequest(void){
    
  }
  
  public: void Start(void)
  {      
    taskFinished = false;
    //Start Task with input parameter set to "this" class
    xTaskCreatePinnedToCore(
      this->httpsTask,        //Function to implement the task 
      "httpsTask",            //Name of the task
      5000,                   //Stack size in words 
      this,                   //Task input parameter 
      taskPriority,           //Priority of the task 
      &Task1,                 //Task handle.
      taskCore);              //Core where the task should run 
  }
  
  
  private: static void httpsTask(void *pvParameters)
  {   
    AsyncHTTPSRequest *l_pThis = (AsyncHTTPSRequest *) pvParameters;   
    // Handle the request here

    delay(1000); //Pretend work
    
    l_pThis->testVar = 1;

    l_pThis->taskFinished = true;
    delay(50); //Wait a little before killing task
    vTaskDelete( NULL );
  }
     
};

AsyncHTTPSRequest req;

void setup() {
  // put your setup code here, to run once:
  //Start Serial Output
  Serial.begin(115200); 
  Serial.println("");
    
  req.Start();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(req.taskFinished){
    Serial.print("Done :");Serial.println(req.testVar);   //Task finished, read result variable
    while(1){delay(10);}
  }
  else
  {
    Serial.println("Busy");
  }

  delay(10);
}