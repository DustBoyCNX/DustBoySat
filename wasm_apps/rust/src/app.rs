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
        // println("Rust is running");
        let led = getPinLED();
        let counter = 0;
        pinMode(led, OUTPUT);
        Self { counter, pm10: 0, pm2_5: 0, pm1: 0 }
    }

    fn run(&mut self) {
        setTitle("DustBoy");
        // setDisplayValue(self.counter.into());
        // print("h,");
        // printInt(self.counter.into());
        // print(",");
        // // printInt(getPm10()); 
        // printInt(self.pm10.into()); 
        // print(",");
        // printInt(self.pm2_5.into());
        // print(",");
        // printInt(self.pm1.into());
        // println("");
        delay(1000);

        self.counter = self.counter + 1;
    }
}

#[no_mangle]
pub extern fn _start() {
    let mut app = App::new();
    loop {
        app.run()
    }
}


#[no_mangle]
pub extern fn _callback() {
    // println("Callback called");
}


#[no_mangle]
pub extern fn updatePmValue(pm10:u16, pm2_5:u16, pm1:u16) {
    println("updatePmValue called");
    print("u,");
    printInt(pm10.into());
    print(",");
    printInt(pm2_5.into());
    print(",");
    printInt(pm1.into());
    println("");
}



// pub extern fn updatePmValue(u16 _pm10, u16 _pm2_5, u16 _pm1) {
//     println("updatePmValue");
//     pm10 = _pm10;
//     // pm2_5 = _pm2_5;
//     // pm1 = _pm1;
// }