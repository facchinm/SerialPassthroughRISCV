/*

  INSTRUCTIONS:
  clone https://github.com/facchinm/riscv-soc-cores and https://github.com/facchinm/bus-spider-firmware/
  cd riscv-soc-cores
  fusesoc --cores-root cores/ build c10lp-evkit-picorv32-wb-soc
  makeCompositeBinary -i build/c10lp-evkit-picorv32-wb-soc_0/bld-quartus/c10lp-evkit-picorv32-wb-soc_0.ttf:1:512 -o ~/Arduino/SerialPassthroughRISCV/app.h  -t 1
  cd .. && cd bus-spider-firmware
  make -s -C bus-spider-firmware CROSS_COMPILE=/run/media/martino/work/riscv32ic/bin/riscv32-unknown-elf- bus_spider.nmon
  ./bus-spider-firmware/nmon-loader bus-spider-firmware/bus_spider.nmon /dev/ttyACM0 115200
  g 40000000

*/

// needs a standalone version of VidorUtils lib
#include "VidorUtils.h"

#if 1
__attribute__ ((used, section(".fpga_bitstream_signature")))
const unsigned char signatures[4096] = {
#include "signature.h"
};
__attribute__ ((used, section(".fpga_bitstream")))
const unsigned char bitstream[] = {
#include "app.h"
};
#endif


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial1.begin(115200);
  //enableFpgaClock();
  pinMode(31, OUTPUT);
  VidorUtils utils;
  utils.begin(true);
  digitalWrite(31, HIGH);
  delay(100);
  digitalWrite(31, LOW);
  delay(100);
  digitalWrite(31, HIGH);
}

void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
}
