# Requires 3 arguments:
# 0: Folder name
# 1: Initial Solution strategy
# 2: Metaheuristic strategy
# 3: Number of runs

# Get arguments
filelist = Dir["#{ARGV[0]}/*"].sort!
initial_solution = ARGV[1]
metaheuristic = ARGV[2]
runs = ARGV[3].to_i
output = ARGV[4]

top = ["Filename","Minimum Solution", "Maximum Solution", "Average Solution","Minimum Time","Maximum Time", "Average Time"]
puts (sprintf("%-35s %-20s %-20s %-20s %-20s %-20s %-20s",top[0],top[1],top[2],top[3],top[4],top[5],top[6])) if output == "summary"

# For each file
filelist.each do |filename|

  if output == "runs"
    puts filename[/(.*\/)(.*)/,2]
    puts "Initial,Solution,Time"
  end

  results = {init: [], final: [], time: []}
  # Run and append a number of programs runs
  runs.times do |run|
    # Obtain result
    result = `./mdp #{filename} #{initial_solution} #{metaheuristic} #{Random::rand 10 ** 8}`
    puts result.split.join(",") if output == "runs"
    # Insert results
    init, final, time = result.split.map { |s| s.to_f }

    results[:init] << init
    results[:final] << final
    results[:time] << time
  end

  # Calculate min, max and avg of solution and time
  minSol = results[:final].min
  maxSol = results[:final].max
  avgSol = results[:final].inject(:+) / results[:final].length
  minTime = results[:time].min
  maxTime = results[:time].max
  avgTime = results[:time].inject(:+) / results[:time].length

  goal = [filename[/(.*\/)(.*)/,2], minSol, maxSol, avgSol, minTime, maxTime, avgTime]
  puts (sprintf("%-35s %-20.4f %-20.4f %-20.4f %-20.4f %-20.4f %-20.4f",goal[0],goal[1],goal[2],goal[3],goal[4],goal[5],goal[6])) if output == "summary"
end
