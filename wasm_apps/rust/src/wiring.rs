#![allow(dead_code)]
#![allow(non_snake_case)]

#[link(wasm_import_module = "wiring")]
extern "C" {
    #[link_name = "millis"]
    fn _millis() -> u32;
    #[link_name = "delay"]
    fn _delay(ms: u32);
    #[link_name = "pinMode"]
    fn _pinMode(pin: u32, mode: u32);
    #[link_name = "digitalWrite"]
    fn _digitalWrite(pin: u32, value: u32);
    #[link_name = "getPinLED"]
    fn _getPinLED() -> u32;
    #[link_name = "getPm10"]
    fn _getPm10() -> u32;
    #[link_name = "getPm1"]
    fn _getPm1() -> u32;
    #[link_name = "getPm2_5"]
    fn _getPm2_5() -> u32;
    #[link_name = "getGreeting"]
    fn _getGreeting(utf8: *mut u8, maxlen: usize);
    #[link_name = "print"]
    fn _print(utf8: *const u8, len: usize);
    #[link_name = "rawSerialOut"]
    fn _rawSerialOut(value: u32);
    #[link_name = "printInt"]
    fn _print_int(out: u32);
}

#[link(wasm_import_module = "display")]
extern "C" {
    #[link_name = "setTitle"]
    fn _setTitle(utf8: *const u8, len: usize);
    #[link_name = "setDisplayValue"]
    fn _setDisplayValue(out: u32);
}

#[link(wasm_import_module = "serial")]
extern "C" {
    //   #[link_name = "print"] fn unsafe_print(ptr: *const u8, size: usize);
    #[link_name = "printInt"]
    fn unsafe_print_int(out: u32);
}

pub static LOW: u32 = 0;
pub static HIGH: u32 = 1;

pub static INPUT: u32 = 0x0;
pub static OUTPUT: u32 = 0x1;
pub static INPUT_PULLUP: u32 = 0x2;

pub fn millis() -> u32 {
    unsafe { _millis() }
}
pub fn delay(ms: u32) {
    unsafe {
        _delay(ms);
    }
}
pub fn pinMode(pin: u32, mode: u32) {
    unsafe { _pinMode(pin, mode) }
}
pub fn digitalWrite(pin: u32, value: u32) {
    unsafe { _digitalWrite(pin, value) }
}
pub fn rawSerialOut(value: u32) {
    unsafe { _rawSerialOut(value) }
}
pub fn getPinLED() -> u32 {
    unsafe { _getPinLED() }
}
pub fn getPm10() -> u32 {
    unsafe { _getPm10() }
}
pub fn getPm1() -> u32 {
    unsafe { _getPm1() }
}
pub fn getPm2_5() -> u32 {
    unsafe { _getPm2_5() }
}

pub fn printInt(out: u32) {
    unsafe {
        _print_int(out);
    }
}
pub fn serialPrintInt(out: u32) {
    unsafe {
        unsafe_print_int(out);
    }
}

pub fn print(string: &str) {
    unsafe { _print(string.as_ptr(), string.len()) }
}

pub fn setTitle(string: &str) {
    unsafe { _setTitle(string.as_ptr(), string.len()) }
}

pub fn setDisplayValue(out: u32) {
    unsafe { _setDisplayValue(out) }
}

pub fn println(string: &str) {
    print(string);
    print("\n");
}

// pub fn getGreeting() {
//     let mut buffer = [0u8; 256];
//     unsafe { _getGreeting(buffer.as_mut_ptr(), buffer.len()) };
//     print("hello greeting! with len: ");
//     printInt(buffer.len() as u32);
//     println(buffer.as_ptr().as_str().unwrap());
//     // String::from_utf8_lossy(&buffer).to_string()
// }

//TODO: getGreeting

#[panic_handler]
fn handle_panic(_: &core::panic::PanicInfo) -> ! {
    unsafe { core::arch::wasm32::unreachable() }
    // core::arch::wasm32::unreachable()
}
