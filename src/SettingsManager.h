#ifndef SETTINGS_MANAGER_H_
#define SETTINGS_MANAGER_H_

#include "ofMain.h"

class SettingsManager
{
private:
  ofXml XML;

public:
  // use the structure below to set all the default values
  struct settings {
    struct {
      int value = 60;
      int min = 30;
      int max = 200;
    } bpm;

    struct {
      string status = "on";
      string listen = "3333";
    } tuio;

    struct {
      string status = "on";
      string host = "localhost";
      string send = "9997";
      string listen = "9998";
    } osc;



    struct {
      struct {
        int r = 200;
        int g = 200;
        int b = 200;
      } background;
      struct {
        int r = 160;
        int g = 160;
        int b = 160;
      } fbo;
      // struct {
      //   int r = 160;
      //   int g = 160;
      //   int b = 160;
      // } playground;
      struct {
        int r = 255;
        int g = 0;
        int b = 255;
      } playhead;
      struct {
        float moveX = 0.0;
        float moveY = 0.0;
        float rotX  = 0.0;
        float rotZ  = 0.0;
        float scale = 1.0;
      } distorsion;
    } aspect;



    struct {

      int size = 20;

      struct {
        int in = 100;
        int out = 250;
      } fadetime;

      struct {

        struct {
          int in = 10;
          int out = 100;
        } triggertime;

        struct {
          int fid = 0;
          struct {
            int r = 255;
            int g = 0;
            int b = 0;
          } color;
        } kick;

        struct {
          int fid = 13;
          struct {
            int r = 0;
            int g = 100;
            int b = 255;
          } color;
        } snare;

        struct {
          int fid = 49;
          struct {
            int r = 100;
            int g = 255;
            int b = 0;
          } color;
        } hihat;

      } percussions;

      struct {

        struct {
          int fid = 39;
          struct {
            int r = 0;
            int g = 0;
            int b = 0;
          } color;
        } chord1;

        struct {
          int fid = 47;
          struct {
            int r = 20;
            int g = 20;
            int b = 20;
          } color;
        } chord2;

        struct {
          int fid = 60;
          struct {
            int r = 40;
            int g = 40;
            int b = 40;
          } color;
        } chord3;

        struct {
          int fid = 63;
          struct {
            int r = 60;
            int g = 60;
            int b = 60;
          } color;
        } chord4;

        struct {
          int fid = 79;
          struct {
            int r = 80;
            int g = 80;
            int b = 80;
          } color;
        } chord5;

        struct {
          int fid = 84;
          struct {
            int r = 100;
            int g = 100;
            int b = 100;
          } color;
        } chord6;

      } chords;

      struct {
        int fid = 67;
        struct {
          int r = 100;
          int g = 255;
          int b = 0;
        } color;
        struct {
          float angleRange = 1.0;
          float angleGap = 1.0;
          int radius = 30;
          int w = 15;
          struct {
            int r = 120;
            int g = 0;
            int b = 0;
          } backColor;
          struct {
            int r = 255;
            int g = 0;
            int b = 0;
          } frontColor;
        } meter;
      } metro;

      struct {
        int fid = 15;
        struct {
          int r = 0;
          int g = 255;
          int b = 0;
        } color;
      } bass;

      struct {
        int fid = 26;
        struct {
          int r = 255;
          int g = 0;
          int b = 0;
        } color;
      } pad;

    } fiducials;
  } defaults, customs;

  SettingsManager();
  ~SettingsManager();

  void setup();

  void loadDefaults(); // load default settings
  void saveSettings(); // save setting to XML file
  void loadSettings(); // load settings from XML file
  void printSettings();

  // struct settings * getDefaults();  // apparently not used by anyone
  struct settings * getCustoms(); // called by the main thread
};

#endif /* end of SETTINGS_MANAGER_H_ */
