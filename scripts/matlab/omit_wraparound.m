function x = omit_wraparound(a, nc_idx, mode, num_bits)

if strcmp (mode, "twoscomplement")
    limit_bot = -2^(num_bits-1);
    limit_up  =  2^(num_bits-1)-1;
elseif strcmp (mode, "offsetbinary")
    limit_bot = 0;
    limit_up  = 2^(num_bits)-1;
else
    error ("mode is not valid! Supported modes are \"twoscomplement\" and \"offsetbinary\"");
end

x = [];
for n = 1:2:size(nc_idx,1)*size(nc_idx,2)
    if a(nc_idx(n)) != limit_up || a(nc_idx(n+1)) != limit_bot
        x = [x, [nc_idx(n);nc_idx(n+1)]];
    end
end

end
