 void alexasetup(){
  // para integrar en Amazon alexa /////////////////////////////////////////////////
//    server.on("/", HTTP_GET, [](){
//    server.send(200, "text/plain", "This is an example index page your server may send.");
//    });
    server.on("/test", HTTP_GET, [](){
    server.send(200, "text/plain", "This is a second subpage you may have.");
    });
    server.onNotFound([](){
      if (!espalexa.handleAlexaApiCall(server.uri(),server.arg(0))) //if you don't know the URI, ask espalexa whether it is an Alexa control request
      {
        //whatever you want to do with 404s
        server.send(404, "text/plain", "Not found");
      }
    });

    // Define your devices here.
    espalexa.addDevice("Toldo", ToldoChanged); //simplest definition, default state off

    espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
    //server.begin(); //omit this since it will be done by espalexa.begin(&server)
  
  }

  //our callback functions
void ToldoChanged(uint8_t brightness) {
    Serial.print("Alexa: Toldo changed to ");
    
    //do what you need to do here

    //EXAMPLE
    //if (brightness == 255) {
    if (brightness > 200) {
        Serial.println("ON");
        estado_anterior=0; //por si acaso el programa se ha reseteado, cambio el estado
        estados[0]=0; //en control por voz se supone que se ve lo que hay
        AbrirToldo(0);
    }
    else if (brightness == 0) {
        Serial.println("OFF");
        estado_anterior=1;
        estados[0]=0;
        CerrarToldo(0);
    }
    else {
      Serial.print("DIM "); Serial.println(brightness);
      AbrirToldo_parcial(brightness); //porcentaje en brillo de 0 a 255
    }
}
