import processing.serial.*;


/**
 * Ejemplo de skecth Processing para el desarrollo del Laboratorio.
 *
 * Este skecth implementa un programa en el host que tiene la capacidad de
 * graficar funciones que evolucionan en el tiempo y además permite la
 * presentación de datos simples mediante Labels, y la captura de eventos
 * del usuario mediante la implementación de botones simples.
 *
 * La aplicación divide la ventana en 2 regiones, una de dibujado y otra 
 * donde se ubican los botones y etiquetas de información.
 *
 */


// Declaraciones para graficar funciones...
int cantValues;
int sampling_delay;
ScrollingFcnPlot f_temp_act, f_temp_max, f_temp_min, f_temp_avg;

//Botones...
RectButton actBtn, maxBtn, minBtn, avgBtn;
PFont myFont;

//Etiquetas textuales...
Label lbl1, lbl2, lbl3, lbl4, lbl5, lbl6, lbl7, lbl8, lbl9, lbl10, lbl11;
Label yVal1, yVal2, yVal3;

//Ventana y viewports...
int pgFunctionViewportWidth = 480;
int pgControlViewportWidth = 120;
int pgViewportsHeight = 240;

Serial myPort;  // Create object from Serial class
String val = "";     // Data received from the serial port
float temp_act, temp_max, temp_min, temp_avg, aux;
int mode = 0;
int time; //tiempo desde la ultima lectura de temperaturas
int oldTime;


void setup() {

  //Se define el tamaño de la ventana de la aplicación... 
  size(600, 240);

  //Se inicializan los arreglos para almacenar las funciones...
  cantValues = pgFunctionViewportWidth;

  sampling_delay = (1000*60)/cantValues; // delay necesario para muestrear durante un minuto

  f_temp_act = new ScrollingFcnPlot(cantValues, color(255, 255, 255), -40, 40);
  f_temp_max = new ScrollingFcnPlot(cantValues, color(255, 0, 0), -40, 40);
  f_temp_min = new ScrollingFcnPlot(cantValues, color(0, 0, 255), -40, 40);
  f_temp_avg = new ScrollingFcnPlot(cantValues, color(0, 255, 0), -40, 40);

  //Se inicializan los botones...
  actBtn = new RectButton(pgFunctionViewportWidth+5, 10, 40, 40, color(102), color(50), color(255), "act");
  maxBtn = new RectButton(pgFunctionViewportWidth+5, 60, 40, 40, color(102), color(50), color(255), "max");
  minBtn = new RectButton(pgFunctionViewportWidth+60, 10, 40, 40, color(102), color(50), color(255), "min");
  avgBtn = new RectButton(pgFunctionViewportWidth+60, 60, 40, 40, color(102), color(50), color(255), "avg");

  //Se inicializan los labels...
  lbl1 = new Label(pgFunctionViewportWidth+10, 110, color(255), "Status: ");
  lbl2 = new Label(pgFunctionViewportWidth+60, 110, color(255), "-");
  lbl3 = new Label(pgFunctionViewportWidth+10, 130, color(255), "T.Actual: ");
  lbl4 = new Label(pgFunctionViewportWidth+70, 130, color(255), "30.00ºC");
  lbl5 = new Label(pgFunctionViewportWidth+10, 150, color(255, 0, 0), "T.Max: ");
  lbl6 = new Label(pgFunctionViewportWidth+70, 150, color(255, 0, 0), "30.00ºC");
  lbl7 = new Label(pgFunctionViewportWidth+10, 170, color(0, 0, 255), "T.Min: ");
  lbl8 = new Label(pgFunctionViewportWidth+70, 170, color(0, 0, 255), "30.00ºC");
  lbl9 = new Label(pgFunctionViewportWidth+10, 190, color(0, 255, 0), "T.Prom: ");
  lbl10 = new Label(pgFunctionViewportWidth+70, 190, color(0, 255, 0), "30.00ºC");
  lbl11 = new Label(pgFunctionViewportWidth+10, 210, color(255, 0, 0), "ALERTA!!!");

  yVal1 = new Label(10, 5, color(255), "40");
  yVal2 = new Label(10, (pgViewportsHeight-20)/2, color(255), "0");
  yVal3 = new Label(10, pgViewportsHeight-25, color(255), "-40");

  //Inicializa el font de la GUI...
  myFont = createFont("FFScala", 14);
  textFont(myFont);

  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
}

void draw() {
  //utilizamos informacion del timer para actualizar el grafico cada 125ms aproximadamente
  int timeDelta = millis() - oldTime;
  oldTime = millis();
  time = time + timeDelta;

  //Se actualizan los valores de la temperatura
  myPort.write((byte)(mode));
  aux = readFloat();
  if (aux > -99.0) {
    switch (mode) {
    case 0:
      temp_avg = aux;
      lbl10.caption = temp_avg + "ºC";
      break;
    case 1:
      temp_act = aux;
      lbl4.caption = temp_act + "ºC";
      break;
    case 2:
      temp_max = aux;
      lbl6.caption = temp_max + "ºC";
      break;
    case 3:
      temp_min = aux;
      lbl8.caption = temp_min + "ºC";
      break;
    }
  }
  mode++;
  mode = mode % 4;

  //si paso el tiempo necesario actualizamos los elementos graficos
  if (time > sampling_delay) {
    time = time - sampling_delay;

    //actualizamos los valores de las functiones
    f_temp_act.updateValue(temp_act);
    f_temp_max.updateValue(temp_max);
    f_temp_min.updateValue(temp_min);
    f_temp_avg.updateValue(temp_avg);

    //Rendering de la interface...
    background(125);
    stroke(0);
    noFill();
    //Dibuja las funciones...
    f_temp_act.displayIntoRect(30, 10, pgFunctionViewportWidth-10, pgViewportsHeight-10);
    f_temp_max.displayIntoRect(30, 10, pgFunctionViewportWidth-10, pgViewportsHeight-10);
    f_temp_min.displayIntoRect(30, 10, pgFunctionViewportWidth-10, pgViewportsHeight-10);
    f_temp_avg.displayIntoRect(30, 10, pgFunctionViewportWidth-10, pgViewportsHeight-10);

    //Dibuja el eje X y el recuadro de los gráficos...
    stroke(0);
    line(30, pgViewportsHeight/2, pgFunctionViewportWidth-10, pgViewportsHeight/2);
    rect(30, 10, pgFunctionViewportWidth-40, pgViewportsHeight-20);

    //Se dibujan los botones...
    actBtn.display();
    maxBtn.display();
    minBtn.display();
    avgBtn.display();

    //Se dibuja texto adicional...(labels, etc)
    lbl1.display();
    lbl2.display();
    lbl3.display();
    lbl4.display();
    lbl5.display();
    lbl6.display();
    lbl7.display();
    lbl8.display();
    lbl9.display();
    lbl10.display();


    yVal1.display();
    yVal2.display();
    yVal3.display();
  }
  //Procesa eventos de MouseOver...
  actBtn.update();
  maxBtn.update();
  minBtn.update();
  avgBtn.update();
}

void mouseClicked()
{
  if (actBtn.over()) {
    actBtn.currentcolor = color(0, 100, 0);
    lbl2.caption = "act";
    myPort.write((byte)4);
  } else if (maxBtn.over()) {
    maxBtn.currentcolor = color(0, 100, 0);
    lbl2.caption = "max";
    myPort.write((byte)5);
  } else if (minBtn.over()) {
    minBtn.currentcolor = color(0, 100, 0);
    lbl2.caption = "min";
    myPort.write((byte)6);
  } else if (avgBtn.over()) {
    avgBtn.currentcolor = color(0, 100, 0);
    lbl2.caption = "avg";
    myPort.write((byte)7);
  }
}

float readFloat() { 
  float toReturn = -99.0;
  while (myPort.available() > 0) {
    val = myPort.readStringUntil('\n');
    if (val != null) {
      toReturn = float(trim(val));
    }
  }
  myPort.clear();
  return toReturn;
}
