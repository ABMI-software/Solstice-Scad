#ifdef SENSORS_H
#define SENSORS_H

#include <max6675.h>
#include "pins.h"
#include "config.h"

Class SensorModule{
    public:
        SensorModule();
        ~SensorModule();
        void begin();
        float readAll();
        float getPTC1();
        float getPTC2();
        float getCUVE1();
        float getCUVE2();
        float readAverage();

    private:
        MAX6675 ptc1;
        MAX6675 ptc2;
        MAX6675 cuve1;
        MAX6675 cuve2;

        float ptc1_avg;
        float ptc2_avg;
        float cuve1_avg;
        float cuve2_avg;

        float readings[4];
};

#endif