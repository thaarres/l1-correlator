-- emp_project_decl for the VCU118 minimal example design
--
-- Defines constants for the whole project
--


library IEEE;
use IEEE.STD_LOGIC_1164.all;

use work.emp_framework_decl.all;
use work.emp_device_types.all;


package emp_project_decl is

  constant PAYLOAD_REV : std_logic_vector(31 downto 0) := X"7EA00001";

  -- Number of LHC bunches 
  constant LHC_BUNCH_COUNT : integer := 3564;
  -- Latency buffer size
  constant LB_ADDR_WIDTH   : integer := 10;

  -- Clock setup
  constant CLOCK_COMMON_RATIO : integer             := 36;
  constant CLOCK_RATIO        : integer             := 9;
  constant CLOCK_AUX_RATIO    : clock_ratio_array_t := (2, 4, 9);

  -- Only used by nullalgo
  constant PAYLOAD_LATENCY : integer := 5;

  constant REGION_CONF : region_conf_array_t := (
    0      => (no_mgt, buf, no_fmt, buf, no_mgt),
    1      => (no_mgt, buf, no_fmt, buf, no_mgt),
    2      => (no_mgt, buf, no_fmt, buf, no_mgt),
    3      => (no_mgt, buf, no_fmt, buf, no_mgt),
    4      => kDummyRegion,
    5      => kDummyRegion,
    6      => (no_mgt, buf, no_fmt, buf, no_mgt),
    7      => (no_mgt, buf, no_fmt, buf, no_mgt),
    8      => (no_mgt, buf, no_fmt, buf, no_mgt),
    9      => (no_mgt, buf, no_fmt, buf, no_mgt),
    10     => (no_mgt, buf, no_fmt, buf, no_mgt),
    11     => (no_mgt, buf, no_fmt, buf, no_mgt),
    12     => (no_mgt, buf, no_fmt, buf, no_mgt),
    13     => (no_mgt, buf, no_fmt, buf, no_mgt),
    14     => (no_mgt, buf, no_fmt, buf, no_mgt),
    15     => (no_mgt, buf, no_fmt, buf, no_mgt),
    16     => (no_mgt, buf, no_fmt, buf, no_mgt),
    17     => (no_mgt, buf, no_fmt, buf, no_mgt),
    18     => (no_mgt, buf, no_fmt, buf, no_mgt),
    19     => (no_mgt, buf, no_fmt, buf, no_mgt),
    20     => (no_mgt, buf, no_fmt, buf, no_mgt),
    21     => (no_mgt, buf, no_fmt, buf, no_mgt),
    22     => (no_mgt, buf, no_fmt, buf, no_mgt),
    23     => (no_mgt, buf, no_fmt, buf, no_mgt),
    24     => (no_mgt, buf, no_fmt, buf, no_mgt),
    25     => (no_mgt, buf, no_fmt, buf, no_mgt),
    26     => (no_mgt, buf, no_fmt, buf, no_mgt),
    27     => (no_mgt, buf, no_fmt, buf, no_mgt),
    ---- Cross-chip
    others => kDummyRegion
    );

end emp_project_decl;