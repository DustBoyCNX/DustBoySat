#![no_std]

mod wiring;
use wiring::*;

struct App {
    counter: u8,
    pm10: u16,
    pm2_5: u16,
    pm1: u16,
}

impl App {
    fn new() -> Self {
        let led = getPinLED();
        let counter = 0;
        pinMode(led, OUTPUT);
        Self {
            counter,
            pm10: 0,
            pm2_5: 0,
            pm1: 0,
        }
    }

    fn run(&mut self) {
        setTitle("DustBoy");
        delay(1000);

        self.counter += 1;
    }
}

#[no_mangle]
pub extern "C" fn _start() {
    let mut app = App::new();
    loop {
        app.run()
    }
}

#[no_mangle]
pub extern "C" fn _callback() {}

#[no_mangle]
pub extern "C" fn updatePmValue(pm10: u16, pm2_5: u16, pm1: u16) {
    println("updatePmValue called");
    print("u,");
    printInt(pm10.into());
    print(",");
    printInt(pm2_5.into());
    print(",");
    printInt(pm1.into());
    println("");
}
