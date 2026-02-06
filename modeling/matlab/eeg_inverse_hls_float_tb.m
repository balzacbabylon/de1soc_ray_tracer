function eeg_inverse_hls_float_tb()
%#codegen

    Ns = 900;
    B  = 1;
    N_elec = 29;

    Y_block = zeros(N_elec, B);
    K       = zeros(Ns, N_elec);

    % Simple deterministic stimuli (HDL-safe)
    for c = 1:N_elec
        for b = 1:B
            Y_block(c,b) = c;
        end
    end

    for s = 1:Ns
        for c = 1:N_elec
            K(s,c) = c;
        end
    end

    J_block = eeg_inverse_hls_float(Y_block, K);
end