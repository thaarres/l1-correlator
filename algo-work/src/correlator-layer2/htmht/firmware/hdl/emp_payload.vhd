-- null_algo
--
-- Do-nothing top level algo for testing
--
-- Dave Newbold, July 2013

library IEEE;
use IEEE.STD_LOGIC_1164.all;

use work.ipbus.all;
use work.emp_data_types.all;
use work.emp_project_decl.all;

use work.emp_device_decl.all;
use work.emp_ttc_decl.all;

entity emp_payload is
  port(
    clk         : in  std_logic;        -- ipbus signals
    rst         : in  std_logic;
    ipb_in      : in  ipb_wbus;
    ipb_out     : out ipb_rbus;
    clk_payload : in  std_logic_vector(2 downto 0);
    rst_payload : in  std_logic_vector(2 downto 0);
    clk_p       : in  std_logic;        -- data clock
    rst_loc     : in  std_logic_vector(N_REGION - 1 downto 0);
    clken_loc   : in  std_logic_vector(N_REGION - 1 downto 0);
    ctrs        : in  ttc_stuff_array;
    bc0         : out std_logic;
    d           : in  ldata(4 * N_REGION - 1 downto 0);  -- data in
    q           : out ldata(4 * N_REGION - 1 downto 0);  -- data out
    gpio        : out std_logic_vector(29 downto 0);  -- IO to mezzanine connector
    gpio_en     : out std_logic_vector(29 downto 0)  -- IO to mezzanine connector (three-state enables)
    );

end emp_payload;

architecture rtl of emp_payload is

begin
  ipb_out <= IPB_RBUS_NULL;
  bc0 <= '0';
  gpio    <= (others => '0');
  gpio_en <= (others => '0');
  
  
  HTMHTInstance : entity work.algo_main
  port map (
          ap_clk => clk_p,
          ap_rst => '0',
          ap_start => '1',
          jets_0 => d(0).data(36 downto 0),
          jets_1 => d(1).data(36 downto 0),
          jets_2 => d(2).data(36 downto 0),
          jets_3 => d(3).data(36 downto 0),
          jets_4 => d(4).data(36 downto 0),
          jets_5 => d(5).data(36 downto 0),
          jets_6 => d(6).data(36 downto 0),
          jets_7 => d(7).data(36 downto 0),
          jets_8 => d(8).data(36 downto 0),
          jets_9 => d(9).data(36 downto 0),
          ht_hwHt_V => q(0).data(13 downto 0),
          ht_hwMHt_V => q(0).data(27 downto 14),
          ht_hwPhi_V => (others => '0')
        );
        
  q(0).strobe <= '1';

end rtl;
