#![no_std]

mod wiring;
use wiring::*;

struct App {
    led: u32,
}

impl App {
    fn new() -> Self {
        println("Rust is running 😎");

        let led = getPinLED();
        pinMode(led, OUTPUT);
        Self { led }
    }

    fn run(&self) {
        print("h,");
        printInt(getPm2_5());
        print(",");
        printInt(getPm2_5());
        print(",");
        printInt(getPm2_5());
        println("");
        delay(1000);
    }
}

#[no_mangle]
pub extern fn _start() {
    let app = App::new();
    loop {
        app.run()
    }
}


#[no_mangle]
pub extern fn _callback() {
    println("Callback called");
}
