#####Last updated for Add-on API v4.0

UnitTestC Add-on
---

Designed to test against Add-on API for any possible exploit and apply internal fixes to Halo Extension. You will need to uncomment specific definition in order to use specific API to test against. Here's the proceedure of how to perform a test.

1. Uncomment specific definition you want to test against API by following location below.
  * UnitTestC.c file.
2. Compile it.
3. Use Add-on Converter application to convert it into eao format.
4. Copy UnitTestC.eao file into H-Ext's plugins folder.
5. Start up any Halo (Windows) version if you haven't done so.
6. Load H-Ext if you haven't done so.
7. Type `ext_addon_load UnitTestC` in the console.
8. You will receive pass/fail messagebox for each API. If you have found a failure, proceed to next step or skip to step 10.
9. Create an Issue/Ticket report of the failure.
10. Repeat step 1 if want to test different API or repeat step 4 for different Halo version.

**NOTICE: Timer API may will report a failure, this is expected since ticks are not very sane between map change or duration of map running. All issue relative this will not be resolve.**
