function x = find_non_consecutive(a)

p = find(diff(a)!=1);
x = [p;p+1];

end
