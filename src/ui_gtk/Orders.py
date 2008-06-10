# coding=utf-8


# Copyright 2008 Tomi Jylhä-Ollila
#
# This file is part of Kunquat.
#
# Kunquat is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Kunquat is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Kunquat.  If not, see <http://www.gnu.org/licenses/>.


import pygtk
pygtk.require('2.0')
import gtk
import gobject

import liblo


SUBSONGS_MAX = 256
ORDERS_MAX = 256


class Orders(gtk.ScrolledWindow):

	def create_list(self, number):
		subsong = gtk.VBox()
		label = gtk.Label('Subsong %d' % number)
		subsong.pack_start(label, False, False)
		label.show()
		list = gtk.ListStore(str, str)
		for i in range(ORDERS_MAX):
			iter = list.append()
			list.set(iter, 0, '%02X' % i)
		list_view = gtk.TreeView(list)
		cell = gtk.CellRendererText()
		col = gtk.TreeViewColumn('#', cell, text=0)
		list_view.append_column(col)
		cell = gtk.CellRendererText()
		cell.subsong = number
		cell.set_property('editable', True)
		cell.connect('edited', self.order_changed)
		col = gtk.TreeViewColumn('Pattern', cell, text=1)
		list_view.append_column(col)
		list_scroll = gtk.ScrolledWindow()
		list_scroll.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
		list_scroll.add(list_view)
		list_view.show()
		subsong.pack_start(list_scroll)
		list_scroll.show()
		subsong.number = number
		return subsong

	def order_changed(self, cell, path, new_entry):
		order = -1
		if new_entry.isdigit():
			order = int(new_entry)
		elif new_entry != '' and not isspace(new_entry):
			return True
		liblo.send(self.engine, '/kunquat/set_order',
				self.song_id,
				cell.subsong,
				int(path),
				order)

	def order_info(self, path, args, types):
		subsong = None
		if len(args) > 1 or args[0] == 0 or type(self.order_data[args[0]]) == gtk.VBox:
			if not self.order_data[args[0]]:
				subsong = self.create_list(args[0])
				if type(self.order_data[args[0]]) == gtk.Button:
					self.subsongs.remove(self.order_data[args[0]])
				self.order_data[args[0]] = subsong
				self.subsongs.pack_start(subsong)
				subsong.show()
				if args[0] < SUBSONGS_MAX - 1 and not self.order_data[args[0] + 1]:
					button = gtk.Button('New subsong')
					self.order_data[args[0] + 1] = button
					button.number = args[0] + 1
					self.subsongs.pack_start(button, False, False)
					button.connect('clicked', self.new_subsong)
					button.show()
			list = self.order_data[args[0]].get_children()[1].get_child().get_model()
			for i in range(ORDERS_MAX):
				iter = list.get_iter(i)
				list.set_value(iter, 1, '')
			order_desc = args[1:]
			while order_desc:
				order = order_desc.pop(0)
				pattern = order_desc.pop(0)
				if pattern == -1:
					pattern = ''
				else:
					pattern = str(pattern)
				iter = list.get_iter(order)
				list.set_value(iter, 1, pattern)
		"""
		elif type(self.order_data[args[0] - 1]) == gtk.VBox:
			layout_changed = True
			subsong = gtk.Button('New subsong')
			self.order_data[args[0]] = subsong
			subsong.number = args[0]
			self.subsongs.pack_start(subsong, False, False)
			subsong.show()
		if layout_changed:
			self.subsongs = gtk.HBox()
			for sub in self.order_data:
				if sub:
					print(sub)
					self.subsongs.pack_start(sub, False, False)
					sub.show()
			if self.get_child():
				self.remove(self.get_child())
			self.add_with_viewport(self.subsongs)
			self.subsongs.show()
		"""

	"""
	def view_subsongs(self):
		self.subsongs = gtk.HBox()
		self.subsongs.set_spacing(10)
		prev_exists = False
		for i in range(SUBSONGS_MAX):
			if self.order_data[i] or i == 0:
				subsong = gtk.VBox()
				label = gtk.Label('Subsong %d' % i)
				subsong.pack_start(label, False, False)
				label.show()
				list = gtk.ListStore(str, str)
				for i in range(256):
					iter = list.append()
					list.set(iter, 0, '%02X' % i)
				list_view = gtk.TreeView(list)
				cell = gtk.CellRendererText()
				col = gtk.TreeViewColumn('#', cell, text=0)
				list_view.append_column(col)
				cell = gtk.CellRendererText()
				cell.set_property('editable', True)
				col = gtk.TreeViewColumn('Pattern', cell, text=1)
				list_view.append_column(col)
				list_scroll = gtk.ScrolledWindow()
				list_scroll.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
				list_scroll.add(list_view)
				list_view.show()
				subsong.pack_start(list_scroll)
				list_scroll.show()
				self.subsongs.pack_start(subsong)
				subsong.show()
				prev_exists = True
			elif prev_exists:
				prev_exists = False
				subsong = gtk.VBox()
				button = gtk.Button('New subsong')
				subsong.pack_start(button, False, False)
				button.show()
				self.subsongs.pack_start(subsong)
				subsong.show()
				break
		if self.get_children():
			self.remove(self.get_children()[0])
		self.add_with_viewport(self.subsongs)
		self.subsongs.show()
	"""

	def new_subsong(self, button):
		num = button.number
		subsong = self.create_list(num)
		assert button == self.order_data[num]
		self.subsongs.remove(button)
		self.order_data[num] = subsong
		self.subsongs.pack_start(subsong)
		subsong.show()
		if num < SUBSONGS_MAX - 1 and not self.order_data[num + 1]:
			button = gtk.Button('New subsong')
			self.order_data[num + 1] = button
			button.number = num + 1
			self.subsongs.pack_start(button, False, False)
			button.connect('clicked', self.new_subsong)
			button.show()

	def __init__(self, engine, server, song_id):
		self.engine = engine
		self.server = server
		self.song_id = song_id

		self.order_data = [None for _ in range(SUBSONGS_MAX)]

		gtk.ScrolledWindow.__init__(self)
		self.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_NEVER)

		self.subsongs = gtk.HBox()
		self.subsongs.set_spacing(10)
		self.add_with_viewport(self.subsongs)
		self.subsongs.show()
		liblo.send(self.engine, '/kunquat/get_orders', self.song_id)


