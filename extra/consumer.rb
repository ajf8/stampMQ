# Copyright 2013 ajf http://github.com/ajf8
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
require 'rubygems'
require 'stomp'

queue = "/queue/test"

puts "Consumer for queue #{queue}"

client = Stomp::Client.new "admin", "secret", "localhost", 8080, true
#client = Stomp::Client.new( :hosts => [ {:login => "admin", :passcode => "secret", :host => "localhost", :port => 61616, :ssl => true } ] )

client.subscribe queue, { :ack => :client } do | message |
  puts "message=#{message.body}"
  client.acknowledge message # tell the server the message was handled and to dispose of it
end
client.join # without this the above subscription is not activated.
client.close
