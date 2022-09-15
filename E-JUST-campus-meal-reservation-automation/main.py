# -*- coding: utf-8 -*-
"""
Created on Tue Mar  8 14:49:58 2022

@author: A. Aboelnaga
"""
from selenium import webdriver
from selenium.webdriver.support.ui import Select


driver = webdriver.Chrome()

def campus_login(email,password):
  driver.get ('https://ejust-campus.ejust.edu.eg/login')
  driver.implicitly_wait(5)
  driver.find_element_by_id("email").send_keys(email)
  driver.find_element_by_id('password').send_keys(password)
  driver.find_element_by_xpath("//button[@type='submit']").click()
  driver.get("https://ejust-campus.ejust.edu.eg/add/applicant/nutrition/reservation")
  select = Select(driver.find_element_by_id("breakfast_meal"))
  select.select_by_value('1')
  select = Select(driver.find_element_by_id("lunch_meal"))
  select.select_by_value('1')
  select = Select(driver.find_element_by_id("dinner_meal"))
  select.select_by_value('1')
  driver.find_element_by_xpath("//button[@type='submit']").click()


dic = {"email":"password",
       'email2':"password",
       }
for key in dic: 
  campus_login(key,dic[key])


driver.quit()