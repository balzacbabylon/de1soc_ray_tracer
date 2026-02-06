function J_block = eeg_inverse_hls_float(Y_block, K)
    %#codegen
    
    % Y_block: [64 x block_size]
    % K:       [Ns x 64]
    % J_block: [Ns x block_size]
    
    Ns = 900;
    B  = 1;
    N_elec = 29;
    
    J_block = zeros(Ns, B);
    
    for b = 1:B
        %#pragma HLS PIPELINE
        for s = 1:Ns
            %#pragma HLS PIPELINE
            acc = 0;
            for c = 1:N_elec
                %#pragma HLS UNROLL factor=8
                acc = acc + K(s,c) * Y_block(c,b);
            end
            J_block(s,b) = acc;
        end
    end
end