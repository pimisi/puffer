#include <iostream>

#include "trun_box.hh"

using namespace std;
using namespace MP4;

TrunBox::TrunBox(const uint64_t size, const string & type)
  : FullBox(size, type), sample_count_(), duration_(0), sample_size_(0)
{}

void TrunBox::print_structure(const unsigned int indent)
{
  print_size_type(indent);

  string indent_str = string(indent + 2, ' ') + "| ";
  cout << indent_str << "sample count " << sample_count_ << endl;
  cout << indent_str << "duration " << duration_ << endl;
  cout << indent_str << "sample size " << sample_size_ << endl;
}

void TrunBox::parse_data(MP4File & mp4, const uint64_t data_size)
{
  uint64_t init_offset = mp4.curr_offset();

  parse_version_flags(mp4);

  sample_count_ = mp4.read_uint32();

  if (flags() & data_offset_present) {
    mp4.read_uint32();
  }

  if (flags() & first_sample_flags_present) {
    mp4.read_uint32();
  }

  for (uint32_t i = 0; i < sample_count_; ++i) {
    if (flags() & sample_duration_present) {
      duration_ += mp4.read_uint32();
    }

    if (flags() & sample_size_present) {
      sample_size_ += mp4.read_uint32();
    }

    if (flags() & sample_flags_present) {
      mp4.read_uint32();
    }

    if (flags() & sample_composition_time_offsets_present) {
      mp4.read_uint32();
    }
  }

  check_data_left(mp4, data_size, init_offset);
}
