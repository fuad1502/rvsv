package operations;
  virtual class Operations #(
      int XLEN = 32
  );
    static function logic [XLEN-1:0] add_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return val1 + val2;
    endfunction

    static function logic [XLEN-1:0] and_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return val1 & val2;
    endfunction

    static function logic [XLEN-1:0] or_f(input logic [XLEN-1:0] val1, input logic [XLEN-1:0] val2);
      return val1 | val2;
    endfunction

    static function logic [XLEN-1:0] xor_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return val1 ^ val2;
    endfunction

    static function logic [XLEN-1:0] slt_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return {{(XLEN - 1) {1'b0}}, signed'(val1) < signed'(val2)};
    endfunction

    static function logic [XLEN-1:0] sltu_f(input logic [XLEN-1:0] val1,
                                            input logic [XLEN-1:0] val2);
      return {{(XLEN - 1) {1'b0}}, val1 < val2};
    endfunction

    static function logic [XLEN-1:0] sll_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return val1 << val2[4:0];
    endfunction

    static function logic [XLEN-1:0] srl_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return val1 >> val2[4:0];
    endfunction

    static function logic [XLEN-1:0] sra_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return signed'(val1) >>> val2[4:0];
    endfunction

    static function logic [XLEN-1:0] sub_f(input logic [XLEN-1:0] val1,
                                           input logic [XLEN-1:0] val2);
      return val1 - val2;
    endfunction

  endclass : Operations
endpackage
