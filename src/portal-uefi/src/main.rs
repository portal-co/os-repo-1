#![no_main]
#![no_std]

use core::{cell::RefCell, borrow::Borrow};

use alloc::{
    collections::{BTreeMap, BTreeSet},
    rc::Rc, string::String,
};
use log::info;
use uefi::prelude::*;

extern crate more_wasm3;

struct WaCtx {
    pub table: SystemTable<Boot>,
    pub modules: BTreeMap<u64, wasm3::Module>,
    pub heap: BTreeMap<u64, [u8; 65536]>
}
fn boot_blob(x: &str) -> Option<&'static [u8]>{
    match x{
        _ => None
    }
}
fn init<'rt>(mut x: wasm3::Module<'rt>, ctx: Rc<RefCell<WaCtx>>, rt: &'rt wasm3::Runtime) -> u64 {
    let mut i: u64 = 0;

    let mut b = ctx.clone().borrow_mut();
    loop {
        if b.modules.contains_key(&i) {
            i = i + 1;
            continue;
        }
        break;
    }
    drop(b);
    let malloc = x.find_function("malloc").unwrap();
    let free = x.find_function("free").unwrap();
    x.link_closure("portal", "sleep", |a, (t,)| {
        ctx.borrow().table.boot_services().stall(t * 1000);
        return Ok(());
    });
    x.link_closure("portal", "start", |c, (ptr, len)| {
        let x = unsafe { &(&*c.memory())[ptr..ptr + len] };
        let mut p = rt.parse_and_load_module(x)?;
        return Ok(init(p, ctx.clone(), rt));
    });
    x.link_closure("portal", "create_heap", |c, (ptr,)|{
        let mut b = ctx.borrow_mut();
        let x = unsafe {
            arrayref::array_ref![(&*c.memory()),ptr,65536]
        }.clone();
        let mut i = 0;
        loop{
            if b.heap.contains_key(&i){
                i = i + 1;
                continue;
            }
            b.heap.insert(i, x);
            return Ok(i);
        }
    });
    x.link_closure("portal", "read_heap", |c,(h,f):(&u64,u32)|{
        let a: usize = malloc.call(65536)?;
        let mut b = ctx.borrow_mut();
        let h = match f % 2{
            0 => b.heap.remove(h)?,
            1 => b.heap.get(h)?.clone()
        };
        let i = unsafe {
            arrayref::array_mut_ref![(&*c.memory()),a,65536]
        };
        *i = h;
        return Ok(());
    });
    x.link_closure("portal", "ipc", |c,(m,v)|{
        let mut b = ctx.borrow_mut();
        let r: u64 = b.modules.get(m)?.find_function("ipc_endpoint")?.call(v)?;
        Ok(r)
    });

    
    let mut b = ctx.borrow_mut();
    b.modules.insert(i, x);
    return i;
}

#[entry]
fn main(_image_handle: Handle, mut system_table: SystemTable<Boot>) -> Status {
    uefi_services::init(&mut system_table).unwrap();
    system_table.boot_services().set_watchdog_timer(0, 0xffffffff, None);
    info!("Hello world!");
    system_table.boot_services().stall(10_000_000);
    Status::SUCCESS
}
