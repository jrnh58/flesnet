/**
 * @file
 * @author Dirk Hutter <hutter@compeng.uni-frankfurt.de>
 *
 */

#include "device_operator.hpp"
#include "flib.h"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

// measurement interval (equals output interval)
constexpr uint32_t interval_ms = 1000;
constexpr bool clear_screen = true;
constexpr bool detailed_stats = true;

std::ostream& operator<<(std::ostream& os, flib::flib_link::data_sel_t sel) {
  switch (sel) {
  case flib::flib_link::rx_disable:
    os << "disable";
    break;
  case flib::flib_link::rx_emu:
    os << "    emu";
    break;
  case flib::flib_link::rx_link:
    os << "   link";
    break;
  case flib::flib_link::rx_pgen:
    os << "   pgen";
    break;
  default:
    os.setstate(std::ios_base::failbit);
  }
  return os;
}

struct pci_perf_data_t {
  uint64_t cycle_cnt;
  uint64_t pci_stall;
  uint64_t pci_trans;
};

int s_interrupted = 0;
static void s_signal_handler(int signal_value) {
  (void)signal_value;
  s_interrupted = 1;
}

static void s_catch_signals(void) {
  struct sigaction action;
  action.sa_handler = s_signal_handler;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  sigaction(SIGABRT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGINT, &action, NULL);
}

int main(int argc, char* argv[]) {
  s_catch_signals();

  try {

    // display help if any parameter given
    if (argc != 1) {
      (void)argv;
      std::cout
          << "Displays status and performance counters for all FLIB links.\n"
             "Per FLIB counters:\n"
             "idle:     PCIe interface is idle (ratio)\n"
             "stall:    back pressure on PCIe interface from host (ratio)\n"
             "trans:    data is transmitted via PCIe interface (ratio)\n"
             "Per link status/counters:\n"
             "link:     flib/link\n"
             "data_sel: choosen data source\n"
             "up:       flim channel_up\n"
             "he:       aurora hard_error\n"
             "se:       aurora soft_error\n"
             "eo:       eoe fifo overflow\n"
             "do:       data fifo overflow\n"
             "d_max:    maximum number of words in d_fifo\n"
             "dma_s:    stall from dma mux (ratio)\n"
             "data_s:   stall from full data buffer (ratio)\n"
             "desc_s:   stall from full desc buffer (ratio)\n"
             "bp:       back pressure to link (ratio)\n"
             "rate:     ms processing rate (Hz*)\n"
             "* Based on the assumption that the PCIe clock is exactly 100 "
             "MHz.\n"
             "  This may not be true in case of PCIe spread-spectrum "
             "clocking.\n";
      std::cout << std::endl;
      return EXIT_SUCCESS;
    }

    std::unique_ptr<pda::device_operator> dev_op(new pda::device_operator);
    std::vector<std::unique_ptr<flib::flib_device_flesin>> flibs;
    uint64_t num_dev = dev_op->device_count();
    std::vector<flib::dma_perf_data_t> dma_perf_acc(num_dev);
    std::vector<pci_perf_data_t> pci_perf_acc(num_dev);
    std::vector<std::vector<flib::flib_link_flesin::link_perf_t>> link_perf_acc(
        num_dev);

    for (size_t i = 0; i < num_dev; ++i) {
      flibs.push_back(std::unique_ptr<flib::flib_device_flesin>(
          new flib::flib_device_flesin(i)));
      link_perf_acc.at(i).resize(flibs.at(i)->number_of_hw_links());
    }

    // set measurement interval for device and all links
    for (auto& flib : flibs) {
      flib->set_perf_interval(interval_ms);
      flib->get_dma_perf(); // dummy read to reset counters
      for (auto& link : flib->links()) {
        link->set_perf_interval(interval_ms);
      }
    }
    uint32_t pci_cycle_cnt = flibs.at(0)->get_perf_interval_cycles();

    std::cout << "Starting measurements" << std::endl;

    // main output loop
    size_t loop_cnt = 0;
    while (s_interrupted == 0) {
      if (clear_screen) {
        std::cout << "\033\143" << std::flush;
      }
      std::cout << "Measurement " << loop_cnt << ":" << std::endl;
      size_t j = 0;
      for (auto& flib : flibs) {
        uint32_t pci_stall_cyl = flib->get_pci_stall();
        uint32_t pci_trans_cyl = flib->get_pci_trans();
        pci_perf_acc.at(j).cycle_cnt += pci_cycle_cnt;
        pci_perf_acc.at(j).pci_stall += pci_stall_cyl;
        pci_perf_acc.at(j).pci_trans += pci_trans_cyl;

        float pci_max_stall = flib->get_pci_max_stall();
        float pci_stall = pci_stall_cyl / static_cast<float>(pci_cycle_cnt);
        float pci_trans = pci_trans_cyl / static_cast<float>(pci_cycle_cnt);
        float pci_idle = 1 - pci_trans - pci_stall;

        float pci_stall_acc = pci_perf_acc.at(j).pci_stall /
                              static_cast<float>(pci_perf_acc.at(j).cycle_cnt);
        float pci_trans_acc = pci_perf_acc.at(j).pci_trans /
                              static_cast<float>(pci_perf_acc.at(j).cycle_cnt);
        float pci_idle_acc = 1 - pci_trans_acc - pci_stall_acc;

        std::cout << "FLIB " << j << " (" << flib->print_devinfo() << ")"
                  << std::endl;
        std::cout << std::setprecision(4) << "PCIe idle " << std::setw(9)
                  << pci_idle << "   stall " << std::setw(9) << pci_stall
                  << " (max. " << std::setw(5) << pci_max_stall << " us)"
                  << "   trans " << std::setw(9) << pci_trans << std::endl;
        std::cout << std::setprecision(4) << "avg.      " << std::setw(9)
                  << pci_idle_acc << "         " << std::setw(9)
                  << pci_stall_acc << "                "
                  << "         " << std::setw(9) << pci_trans_acc << std::endl;

        if (detailed_stats) {
          flib::dma_perf_data_t dma_perf = flib->get_dma_perf();
          dma_perf_acc.at(j).overflow += dma_perf.overflow;
          dma_perf_acc.at(j).cycle_cnt += dma_perf.cycle_cnt;
          dma_perf_acc.at(j).fifo_fill[0] += dma_perf.fifo_fill[0];
          dma_perf_acc.at(j).fifo_fill[1] += dma_perf.fifo_fill[1];
          dma_perf_acc.at(j).fifo_fill[2] += dma_perf.fifo_fill[2];
          dma_perf_acc.at(j).fifo_fill[3] += dma_perf.fifo_fill[3];
          dma_perf_acc.at(j).fifo_fill[4] += dma_perf.fifo_fill[4];
          dma_perf_acc.at(j).fifo_fill[5] += dma_perf.fifo_fill[5];
          dma_perf_acc.at(j).fifo_fill[6] += dma_perf.fifo_fill[6];
          dma_perf_acc.at(j).fifo_fill[7] += dma_perf.fifo_fill[7];

          std::stringstream ss;
          ss << "fill     1/8     2/8     3/8     4/8     5/8     6/8     7/8  "
                "   8/8    merr"
             << std::endl;
          ss << "    ";
          for (size_t i = 0; i <= 7; ++i) {
            ss << " " << std::setw(7) << std::fixed << std::setprecision(3)
               << dma_perf.fifo_fill[i] / float(dma_perf.cycle_cnt) * 100.0;
          }
          ss << " " << std::setw(7) << dma_perf.overflow << std::endl;
          ss << "avg.";
          for (size_t i = 0; i <= 7; ++i) {
            ss << " " << std::setw(7) << std::fixed << std::setprecision(3)
               << dma_perf_acc.at(j).fifo_fill[i] /
                      float(dma_perf_acc.at(j).cycle_cnt) * 100.0;
          }
          ss << " " << std::setw(7) << dma_perf_acc.at(j).overflow << std::endl;
          std::cout << ss.str();
        }

        ++j;
      }
      std::cout << std::endl;

      std::cout << "link  data_sel  up  d_max        bp         ∅     "
                   "dma_s         ∅    data_s    "
                   "     ∅    desc_s         ∅     rate"
                   "         ∅  he  se  eo  do\n";
      j = 0;
      for (auto& flib : flibs) {
        size_t num_links = flib->number_of_hw_links();
        std::vector<flib::flib_link_flesin*> links = flib->links();

        std::stringstream ss;
        for (size_t i = 0; i < num_links; ++i) {
          flib::flib_link_flesin::link_status_t status =
              links.at(i)->link_status();
          flib::flib_link_flesin::link_perf_t perf = links.at(i)->link_perf();

          link_perf_acc.at(j).at(i).pkt_cycle_cnt += perf.pkt_cycle_cnt;
          link_perf_acc.at(j).at(i).dma_stall += perf.dma_stall;
          link_perf_acc.at(j).at(i).data_buf_stall += perf.data_buf_stall;
          link_perf_acc.at(j).at(i).desc_buf_stall += perf.desc_buf_stall;
          link_perf_acc.at(j).at(i).events += perf.events;
          link_perf_acc.at(j).at(i).gtx_cycle_cnt += perf.gtx_cycle_cnt;
          link_perf_acc.at(j).at(i).din_full_gtx += perf.din_full_gtx;

          float dma_stall =
              perf.dma_stall / static_cast<float>(perf.pkt_cycle_cnt) * 100.0;
          float data_buf_stall = perf.data_buf_stall /
                                 static_cast<float>(perf.pkt_cycle_cnt) * 100.0;
          float desc_buf_stall = perf.desc_buf_stall /
                                 static_cast<float>(perf.pkt_cycle_cnt) * 100.0;
          float din_full = perf.din_full_gtx /
                           static_cast<float>(perf.gtx_cycle_cnt) * 100.0;
          float event_rate =
              perf.events /
              (static_cast<float>(perf.pkt_cycle_cnt) / flib::pkt_clk);

          float dma_stall_acc =
              link_perf_acc.at(j).at(i).dma_stall /
              static_cast<float>(link_perf_acc.at(j).at(i).pkt_cycle_cnt) *
              100.0;
          float data_buf_stall_acc =
              link_perf_acc.at(j).at(i).data_buf_stall /
              static_cast<float>(link_perf_acc.at(j).at(i).pkt_cycle_cnt) *
              100.0;
          float desc_buf_stall_acc =
              link_perf_acc.at(j).at(i).desc_buf_stall /
              static_cast<float>(link_perf_acc.at(j).at(i).pkt_cycle_cnt) *
              100.0;
          float din_full_acc =
              link_perf_acc.at(j).at(i).din_full_gtx /
              static_cast<float>(link_perf_acc.at(j).at(i).gtx_cycle_cnt) *
              100.0;
          float event_rate_acc =
              link_perf_acc.at(j).at(i).events /
              (static_cast<float>(link_perf_acc.at(j).at(i).pkt_cycle_cnt) /
               flib::pkt_clk);

          ss << std::setw(2) << j << "/" << i << "  ";
          ss << std::setw(8) << links.at(i)->data_sel() << "  ";
          // status
          ss << std::setw(2) << status.channel_up << "  ";
          ss << std::setw(5) << status.d_fifo_max_words << "  ";
          // perf counters
          ss << std::setprecision(3); // percision + 5 = width
          ss << std::setw(8) << din_full << "  " << std::setw(8) << din_full_acc
             << "  ";
          ss << std::setw(8) << dma_stall << "  " << std::setw(8)
             << dma_stall_acc << "  ";
          ss << std::setw(8) << data_buf_stall << "  " << std::setw(8)
             << data_buf_stall_acc << "  ";
          ss << std::setw(8) << desc_buf_stall << "  " << std::setw(8)
             << desc_buf_stall_acc << "  ";
          ss << std::setprecision(7) << std::setw(7) << event_rate << "  "
             << std::setw(8) << event_rate_acc << "  ";
          // error
          ss << std::setw(2) << status.hard_err << "  ";
          ss << std::setw(2) << status.soft_err << "  ";
          ss << std::setw(2) << status.eoe_fifo_overflow << "  ";
          ss << std::setw(2) << status.d_fifo_overflow << "  ";

          ss << "\n";
        }
        std::cout << ss.str() << std::endl;
        ++j;
      }
      // sleep will be canceled by signals (which is handy in our case)
      // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66803
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
      ++loop_cnt;
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
