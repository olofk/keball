#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#include <pulse/simple.h>

#include "verilated_vcd_c.h"
#include "Vtop.h"

using namespace std;

static bool done;

vluint64_t main_time = 0;       // Current simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.

double sc_time_stamp () {       // Called by $time in Verilog
  return main_time;           // converts to double, to match
  // what SystemC does
}

void INThandler(int signal)
{
  printf("\nCaught ctrl-c\n");
  done = true;
}

int main(int argc, char **argv, char **env)
{
  Verilated::commandArgs(argc, argv);

  Vtop* top = new Vtop;

  VerilatedVcdC * tfp = 0;

  Verilated::traceEverOn(true);
  tfp = new VerilatedVcdC;
  top->trace (tfp, 99);
  tfp->open ("trace.vcd");

  signal(SIGINT, INThandler);

  static const pa_sample_spec ss = {
    .format = PA_SAMPLE_S16LE,
    .rate = 48000,
    .channels = 2
  };

  #define BUFSIZE 256

  int error;
  uint8_t buf[BUFSIZE];
  pa_simple *s = NULL;

  s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error);

  top->i_clk = 1;
  while (!(done || Verilated::gotFinish())) {

    for (int i=0;i<BUFSIZE;i+=4) {
      top->eval();
      tfp->dump(main_time);
      top->i_clk = !top->i_clk;
      main_time++;
      top->eval();
      tfp->dump(main_time);
      top->i_clk = !top->i_clk;
      main_time++;

      buf[i+0] = (top->o_left >> 0) & 0xff;
      buf[i+1] = (top->o_left >> 8) & 0xff;
      buf[i+2] = (top->o_right >> 0) & 0xff;
      buf[i+3] = (top->o_right >> 8) & 0xff;
    }

    pa_simple_write(s, buf, BUFSIZE, &error);

  }
  pa_simple_drain(s, &error);
  if (s)
    pa_simple_free(s);

  if (tfp)
    tfp->close();
  exit(0);
}
