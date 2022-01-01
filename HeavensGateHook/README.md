# Heaven's Gate Hook

## What And Why?

So I've known about Heaven's Gate Hooking for quite a while now: a way to hook 32 bit syscalls from the usermode 32-to-64 bit translation mechanism.

I wanted to try it myself without any spoilers for the research side of things. So let's give it a try!

## Prior Research

So, I want to study the 32bit to 64bit syscall translation protocol on windows. Let's write a simple 32bit app to call some syscall through ntdll (like sleep?) repeatedly:

```cpp
void main() {
    while (true) {
        Sleep(1000);
        std::cout << "sleeping...\n";
    }
}
```

This does the trick.

Let's open it up in windbg and set a breakpoint on sleep:

```
0:003> bc *
0:003> x *!Sleep
00d12520          TestHookedApp!Sleep (_Sleep@4)
74f34910          KERNELBASE!Sleep (_Sleep@4)
0:003> bp KERNELBASE!Sleep
0:003> g
```



Nice. Now let's wait for a breakpoint and inspect the disassembly:

![image-20211231212027826](C:\Users\alon\source\repos\HeavensGateHook\HeavensGateHook\README.assets\image-20211231212027826.png)

Cool. seems like it's calling `SleepEx(dwMiliseconds, 0)`, which makes sense. lets step in:

![image-20211231212214908](C:\Users\alon\source\repos\HeavensGateHook\HeavensGateHook\README.assets\image-20211231212214908.png)

There's our syscall. Let's step in once more and check it out:

![image-20211231212257900](C:\Users\alon\source\repos\HeavensGateHook\HeavensGateHook\README.assets\image-20211231212257900.png)

Getting warmer... Looks like we're placing some constant (perhaps a syscall number) into eax and a ptr to an ntdll Wow64 syscall transition function to edx and calling it.

![image-20211231212631242](C:\Users\alon\source\repos\HeavensGateHook\HeavensGateHook\README.assets\image-20211231212631242.png)

Looks like all this routine does is jump some ptr global in ntdll. it likely contains the ptr to the Wow64 translation routine.

![image-20211231212749224](C:\Users\alon\source\repos\HeavensGateHook\HeavensGateHook\README.assets\image-20211231212749224.png)

A far jump! interesting...

So we're changing CS from usermode! This new selector effects whether we're executing in 32 bit mode or 64 bit mode:

![image-20211231214100112](C:\Users\alon\source\repos\HeavensGateHook\HeavensGateHook\README.assets\image-20211231214100112.png)

As stated in the intel manuals: `In IA-32e mode, bit 21 of the second doubleword of the segment descriptor indicates whether a code segment contains native 64-bit code. A value of 1 indicates instructions in this code segment are executed in 64-bit mode.`

I think we got enough. By changing the Wow64 transition global from ntdll we can hook heaven's gate in a place common to all 32bit syscalls.