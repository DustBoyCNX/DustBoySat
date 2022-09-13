#![no_std]

mod wiring;
use wiring::*;

struct App {
    led: u32,
    counter: u32,
}

impl App {
    fn new() -> Self {
        println("Rust is running");

        let led = getPinLED();
        let counter = 0;
        pinMode(led, OUTPUT);
        Self { led, counter }
    }

    fn run(&mut self) {

        print("h,");
        printInt(self.counter);
        print(",");
        printInt(getPm10()); 
        print(",");
        printInt(getPm2_5());
        print(",");
        printInt(getPm1());
        println("");
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
    println("Callback called");
}
