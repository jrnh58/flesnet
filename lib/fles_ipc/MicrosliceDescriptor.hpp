// Copyright 2013 Jan de Cuveland <cmail@cuveland.de>
/// \file
/// \brief Defines the fles::MicrosliceDescriptor struct and corresponding
/// enums.
#pragma once

#include <boost/serialization/access.hpp>
#include <cstdint>

/// Main FLES namespace.
namespace fles {

/**
 * \brief Subsystem identifier enum.
 *
 * This enum defines the possible values for the
 * fles::MicrosliceDescriptor::sys_id variable.
 */
enum class SubsystemIdentifier : uint8_t {
  // CBM detectors
  STS = 0x10,  ///< Silicon Tracking System (STS)
  MVD = 0x20,  ///< Micro-Vertex Detector (MVD)
  RICH = 0x30, ///< Ring Imaging CHerenkov detector (RICH)
  TRD = 0x40,  ///< Transition Radiation Detector (TRD)
  MUCH = 0x50, ///< Muon Chamber system (MuCh)
  RPC = 0x60,  ///< Resistive Plate Chambers (RPC)
  ECAL = 0x70, ///< Electromagnetic CALorimeter (ECAL)
  PSD = 0x80,  ///< Projectile Spectator Detector (PSD)

  // Other detectors (experimental)
  TRB3 = 0xE0,      ///< TRB3 Stream
  Hodoscope = 0xE1, ///< Fiber Hodoscope
  Cherenkov = 0xE2, ///< Cherenkov
  LeadGlass = 0xE3, ///< Lead Glas Calorimeter

  // FLES (pattern generators)
  FLES = 0xF0 ///< First-level Event Selector (FLES)
};

enum class SubsystemFormatFLES : uint8_t {
  // FLIB hardware pattern generators
  CbmNetPattern = 0x10,           ///< FLIB hardware pattern generator ("pgen")
  CbmNetFrontendEmulation = 0x11, ///< FLIB front-end emulation ("emu")
  FlibPattern = 0x20,

  // Flesnet software pattern generators
  Uninitialized = 0x80,   ///< Uninitialized data content (without crc)
  BasicRampPattern = 0x81 ///< Basic test pattern (without crc)
};

/**
 * \brief Header format identifier enum.
 *
 * This enum defines the possible values for the
 * fles::MicrosliceDescriptor::hdr_id variable.
 */
enum class HeaderFormatIdentifier : uint8_t { Standard = 0xDD };

/**
 * \brief Header format version enum.
 *
 * This enum defines the possible values for the
 * fles::MicrosliceDescriptor::hdr_ver variable.
 */
enum class HeaderFormatVersion : uint8_t { Standard = 0x01 };

/**
 * \brief Microslice status and error flags.
 *
 * This enum defines the bits in the
 * fles::MicrosliceDescriptor::flags word.
 */
enum class MicrosliceFlags : uint16_t {
  CrcValid = 0x0001,     // information in CRC field is valid
  OverflowFlim = 0x0002, // truncated by FLIM
  OverflowUser = 0x0004  // truncated by user logic
};

#pragma pack(1)

/**
 * \brief %Microslice descriptor struct.
 *
 * This packed struct matches the descriptor generated by the FLIB hardware.
 */
struct MicrosliceDescriptor {
  uint8_t hdr_id;  ///< Header format identifier (0xDD)
  uint8_t hdr_ver; ///< Header format version (0x01)
  uint16_t eq_id;  ///< Equipment identifier
  uint16_t flags;  ///< Status and error flags
  uint8_t sys_id;  ///< Subsystem identifier
  uint8_t sys_ver; ///< Subsystem format/version
  uint64_t idx;    ///< Microslice index / start time
  uint32_t crc;    ///< CRC-32C (Castagnoli polynomial) of data content
  uint32_t size;   ///< Content size (bytes)
  uint64_t offset; ///< Offset in event buffer (bytes)

  friend class boost::serialization::access;
  /// Provide boost serialization access.
  template <class Archive>
  void serialize(Archive& ar, const unsigned int /* version */) {
    ar& hdr_id;
    ar& hdr_ver;
    ar& eq_id;
    ar& flags;
    ar& sys_id;
    ar& sys_ver;
    ar& idx;
    ar& crc;
    ar& size;
    ar& offset;
  }
};

#pragma pack()

} // namespace fles
