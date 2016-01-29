function find_acq_non_consecutive(filename)

data=load(filename);

for n = 1:size(data,2)
    nc = find_non_consecutive(data(:,n)');
    nc_wrap = omit_wraparound(data(:,n)', nc, "twoscomplement", 16)
end

end
