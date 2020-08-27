#
# Copyright 2020-present Open Networking Foundation
#
# SPDX-License-Identifier: LicenseRef-ONF-Member-Only-1.0
#

#TODO Points
#Get port number  as argument 
#IP address looks optional 
import json
import sys
import copy

gauges_family_object_list = [] #type class Gauge
counter_family_object_list = [] #tupe class Counter
module_name = "mmeStats"


#define class 
class Gauge:
    def __init__(self, name, i_labels):
        self.familyname = name
        print("Gauge labels = {} ".format(i_labels))
        #generate enum name 
        e_name = name
        for l in i_labels:
           print("l = {}".format(l)) 
           for k in l.keys():
              v = l[k]
              e_name = e_name + "_" + v
        self.enum_name = e_name.upper()
        print("enum name {}".format(self.enum_name))

        labels = "{"
        for l in i_labels:
            if labels != "{":
              labels = labels + ","
            for k in l.keys():
               v = l[k]
               labels = labels + "{\"" + k + "\",\"" + v + "\"}"
        labels = labels + "}" 
        self.gauge_labels = labels
        print("Gauge labels = {}".format(self.gauge_labels))
            
        gauge_name = "current_"
        labeldict = i_labels
        for l in i_labels:
           for k in l.keys():
              v = l[k]
              gauge_name +=  "_" + v

        self.gauge_name = gauge_name

        self.labeldict = i_labels
        print("Gauge labeldict = {}".format(labeldict))
        print("\n\tGauge \n\t\tFamily {} \n\t\tgauge name - {} \n\t\tlabels {}\n".format(self.familyname, self.gauge_name, self.gauge_labels))

class GaugeFamily:
    def __init__(self, family, familyname, familyhelp, labeldict):
        self.family = family
        self.familyname = familyname
        self.familyhelp = familyhelp
        #self.familylabelk = labelk
        #self.familylabelv = labelv
        print("Gaugefamily labeldict = {}".format(labeldict))
        labels = "{"
        for l in labeldict:
           if labels != "{":
             labels = labels + ","
           for k in l:
              v = l[k]
              labels = labels + "{\"" + k + "\",\"" + v + "\"}"
              labelk = k
              labelv = v
        labels = labels + "}" 

        print("Gaugefamily labels = {}".format(labels))
        self.family_labels = labels
        self.classname = family + "_gauges"
        self.promfamily = family + "_family"
        self.moduleStatsMember = family + "_m"
        self.gaugeMetricList = []
        print("Gaugefamily :  \n\tFamily - {} \n\t\tFamilyName - {} \n\t\tFamilyHelp {} \n\t\tFamilyLabels {} \n\t\tClassname {} \n\t\tPromFamily = {}****** \n".format(self.family, self.familyname, self.familyhelp, self.family_labels, self.classname, self.promfamily))

    def add_gauge(self, family, labels): 
        g = Gauge(family, labels)
        self.gaugeMetricList.append(g)

class Counter:
    def __init__(self, family, metric_name, i_labels):
        print("Counter i_labels = {}".format(i_labels))
        print("Counter __init__ family {}".format(family))
        print("Counter __init__ metric {}".format(metric_name))

        self.family = family
        self.metric_name = metric_name 
        #self.labeldict = labeldict
        e_name = family
        for l in i_labels:
           for v in l.values():
              e_name = e_name + "_" + v
        self.counter_name = e_name.lower()
        self.enum_name = e_name.upper()
        print("enum name {}".format(self.enum_name))
        print("counter name = {}".format(self.counter_name))

        labels = "{"
        for l in i_labels:
           if labels != "{":
             labels = labels + ","
           for k in l.keys():
              v = l[k]
              labels = labels + "{\"" + k + "\",\"" + v + "\"}"
        labels = labels + "}" 
        self.counter_labels = labels

        self.labeldict = copy.deepcopy(i_labels)
        print("final labeldict = {}".format(self.labeldict))
        print("\n\tCounter \n\t\tFamily {} \n\t\tCounterName {} \n\t\tlabels {}\n".format(self.family, self.counter_name, self.counter_labels))

class CounterFamily:
    def __init__(self, family, familyname, familyhelp, labeldict):
        self.family = family
        self.familyname = familyname
        self.familyhelp = familyhelp
        self.classname = family + "_counters"
        self.promfamily = family + "_family"
        self.moduleStatsMember = family + "_m"
        print("CounterFamily labeldict = {}".format(labeldict))
        labels = "{"
        for l in labeldict:
           if labels != "{":
             labels = labels + ","
           print("l = {}".format(l)) 
           for k in l:
              v = l[k]
              labels = labels + "{\"" + k + "\",\"" + v + "\"}"
        labels = labels + "}" 
        self.family_labels = labels
        print("\n**** CounterFamily :  \n\tFamily {} \n\tFamilyname {} \n\tFamilyhelpStr {} \n\tFamilylabels {} \n\tClassname - {} \n\tPromFamily {}******\n".format(self.family, self.familyname, self.familyhelp, self.family_labels, self.classname, self.promfamily))
        self.counterMetricList = []

    def add_counter(self, family, metric_name, labels): 
        c = Counter(family, metric_name, labels)
        self.counterMetricList.append(c)

def add_copyright(fh):
    fh.write("/*\n")
    fh.write(" * Copyright 2020-present Open Networking Foundation\n")
    fh.write(" *\n")
    fh.write(" * SPDX-License-Identifier: Apache-2.0\n")
    fh.write(" */\n")

def add_header_macro(fh):
    h = "#ifndef _INCLUDE_"+module_name+"_H__\n"
    fh.write(h)
    h = "#define _INCLUDE_"+module_name+"_H__\n"
    fh.write(h)

def add_prom_headers(fh):
    fh.write("\n") 
    fh.write("#include <prometheus/gauge.h>\n")
    fh.write("#include <prometheus/counter.h>\n")
    fh.write("#include <prometheus/exposer.h>\n")
    fh.write("#include <prometheus/registry.h>\n")
    fh.write("\n") 

def add_namespace_variables(fh):
    fh.write("\n") 
    fh.write("using namespace prometheus;\n")
    fh.write("extern std::shared_ptr<Registry> registry;\n")
    fh.write("\n") 

def add_setup_function(fh):
    fh.write("\n") 
    fname = module_name + "SetupPrometheusThread"
    declaration = "void " + fname + "(uint16_t port);"
    fh.write(declaration)
    fh.write("\n") 

def add_counter_enum(fh):
    fh.write("\n") 
    fh.write("enum class " + module_name + "Counter {\n")
    enum_class = []
    for gauge in gauges_family_object_list:
      for metric in gauge.gaugeMetricList:
        enum_class.append(metric.enum_name)
    for counter in counter_family_object_list:
      for metric in counter.counterMetricList:
        enum_class.append(metric.enum_name)
    x = len(enum_class)
    for i in range(x-1):
        fh.write("\t" + enum_class[i].upper() + ",\n")
    fh.write("\t" + enum_class[x-1].upper() + "\n")
    fh.write("};\n") 
    fh.write("\n") 
   
def add_dynamic_node_hash_functions(fh):
    fh.write("struct Node \n"
    "{\n")
    fh.write("   " + module_name + "Counter id;\n"
    "	std::string label_k;\n"
    "	std::string label_v;\n"
    "\n")

    fh.write("	Node(" + module_name  + "Counter id, std::string label_k, std::string label_v)\n")
    fh.write("	{\n"
    "		this->id = id;\n"
    "		this->label_k = label_k;\n"
    "		this->label_v = label_v;\n"
    "	}\n"
    "\n"
    "	// operator== is required to compare keys in case of hash collision\n"
    "	bool operator==(const Node &p) const\n"
    "	{\n"
    "		return label_k == p.label_k && label_v == p.label_v && id == p.id;\n"
    "	}\n"
    "};\n"
    "\n"
    "struct hash_fn\n"
    "{\n"
    "	std::size_t operator() (const Node &node) const\n"
    "	{\n"
    "		std::size_t h1 = std::hash<std::string>()(node.label_k);\n"
    "		std::size_t h2 = std::hash<std::string>()(node.label_v);\n"
    "		std::size_t h3 = std::size_t(node.id);\n"
    "		return h1 ^ h2 ^ h3;\n"
    "	}\n"
    "};\n")


def add_footer_macro(fh):
    fh.write("\n") 
    h = "#endif /* _INCLUDE_"+module_name+"_H__ */\n"
    fh.write(h)

def parse_json_file():
  print("sys.argv {}".format(sys.argv))
  file = None
  if(len(sys.argv) == 2):
    file_name = sys.argv[1]
  global module_name
  temp = file_name.split(".")
  module_name = temp[0] 
  with open(file_name) as f:
      data = json.load(f)
  families = data.keys()
  for key in families:
      family = data[key]
      family_name = key 
      print("*********family {} Type - {} *********".format(family_name, family['type']))
      nameStr = family["name"] 
      helpStr = family["help"] 
      print("family_labels {}".format(family['family_labels']))
      flabel_dict = family['family_labels']
     
      if family['type'] == "Gauge":
        gaugeFamilyObj = GaugeFamily(family_name, nameStr, helpStr, flabel_dict) 
        gauges_family_object_list.append(gaugeFamilyObj)
        metrics = family['gauges']
        for metric in metrics:
          label_0_dict = metric["label_0"]
          for zero_label in label_0_dict:
             final_label_dict = []
             zero_label_dict = []
             for temp_zero in zero_label.keys():
                zero_label_dict = [{temp_zero: zero_label[temp_zero]}]
                final_label_dict = copy.deepcopy(zero_label_dict)
                if(metric.get("label_1")):
                  label_1_dict = metric["label_1"]
                  for one_label in label_1_dict:
                     one_label_dict = copy.deepcopy(zero_label_dict)
                     for temp_one in one_label.keys():
                        one_label_dict.append({temp_one: one_label[temp_one]})
                        final_label_dict = copy.deepcopy(one_label_dict)
                        if(metric.get("label_2")):
                          label_2_dict = metric["label_2"]
                          for two_label in label_2_dict:
                             two_label_dict = copy.deepcopy(one_label_dict)
                             for temp_two in two_label.keys():
                                two_label_dict.append({temp_two: two_label[temp_two]})
                                final_label_dict = copy.deepcopy(two_label_dict)
                                print("labels = {}".format(final_label_dict))
                                gaugeFamilyObj.add_gauge(family_name, final_label_dict)
                        else:
                          print("labels = {}".format(final_label_dict))
                          gaugeFamilyObj.add_gauge(family_name, final_label_dict)
                else:
                  print("labels = {}".format(final_label_dict))
                  gaugeFamilyObj.add_gauge(family_name, final_label_dict)

      else:
        counterFamilyObj = CounterFamily(family_name, nameStr, helpStr, flabel_dict) 
        counter_family_object_list.append(counterFamilyObj)
        metrics = family['counters']
        for metric in metrics:
          label_0_dict = metric["label_0"]
          for zero_label in label_0_dict:
             final_label_dict = []
             zero_label_dict = []
             for temp_zero in zero_label.keys():
                zero_label_dict = [{temp_zero: zero_label[temp_zero]}]
                final_label_dict = copy.deepcopy(zero_label_dict)
                if(metric.get("label_1")):
                  label_1_dict = metric["label_1"]
                  for one_label in label_1_dict:
                     one_label_dict = copy.deepcopy(zero_label_dict)
                     for temp_one in one_label.keys():
                        one_label_dict.append({temp_one: one_label[temp_one]})
                        final_label_dict = copy.deepcopy(one_label_dict)
                        if(metric.get("label_2")):
                          label_2_dict = metric["label_2"]
                          for two_label in label_2_dict:
                             two_label_dict = copy.deepcopy(one_label_dict)
                             for temp_two in two_label.keys():
                                two_label_dict.append({temp_two: two_label[temp_two]})
                                final_label_dict = copy.deepcopy(two_label_dict)
                                print("labels = {}".format(final_label_dict))
                                counterFamilyObj.add_counter(family_name, metric['name'], final_label_dict)
                        else:
                          print("labels = {}".format(final_label_dict))
                          counterFamilyObj.add_counter(family_name, metric['name'], final_label_dict)
                else:
                  print("labels = {}".format(final_label_dict))
                  counterFamilyObj.add_counter(family_name, metric['name'], final_label_dict)


def add_gauge_classes(fh):
    fh.write("\n\n")
    fh.write("class DynamicMetricObject" + " {\n")
    fh.write("\tpublic:\n")
    fh.write("};\n")

    for gauge in gauges_family_object_list:
      fh.write("\n\n")
      #1 dynamic label 
      fh.write("class " + gauge.family + "_DynamicMetricObject1 : public DynamicMetricObject" + " {\n")
      fh.write("\tpublic:\n")

      dyn1_fun_signature = {}
      for metric in gauge.gaugeMetricList:
        labels = len(metric.labeldict)
        if(dyn1_fun_signature.get(labels)):
          continue
        constructor_args=""
        gauge_create_labels = "{"
        dynamic_function_call_args=""
        for index in range(labels):
          constructor_args += "std::string label_k"+ str(index) + ", std::string label_v"+ str(index) + ","
          gauge_create_labels += "{" + "label_k"+ str(index) +", label_v" + str(index) + "}"+","
          dynamic_function_call_args += "label_k" + str(index)+", label_v"+str(index) + "," 
        constructor_args += "std::string dlabel_k0, std::string dlabel_v0"
        gauge_create_labels += "{dlabel_k0, dlabel_v0}"
        dynamic_function_call_args += "dlabel_k0, dlabel_v0"
        gauge_create_labels += "}"
        dyn1_fun_signature[labels] = [constructor_args, gauge_create_labels, dynamic_function_call_args]

      for f in dyn1_fun_signature.values():
        constructor_args =f[0]
        gauge_create_labels = f[1]
        dynamic_function_call_args = f[2]
        fh.write("\t\t" + gauge.family + "_DynamicMetricObject1(Family<Gauge> &" + gauge.promfamily + "," + constructor_args +"):\n")
        fh.write("\t\t DynamicMetricObject(),\n")
        fh.write("\t\t gauge(" + gauge.promfamily + ".Add(" + gauge_create_labels + "))\n") 
        fh.write("\t\t{\n")
        fh.write("\t\t}\n")

 
      fh.write("\t\t~" + gauge.family + "_DynamicMetricObject1" + "()\n")
      fh.write("\t\t{\n")
      fh.write("\t\t}\n")
      fh.write("\t\tGauge &gauge;\n")
      fh.write("};\n")
      #end of 1 dynamic arg 

      #2 dynamic label 
      fh.write("\n\n")
      fh.write("\n\n")
      fh.write("class " + gauge.family + "_DynamicMetricObject2 : public DynamicMetricObject" + " {\n")
      fh.write("\tpublic:\n")

      dyn2_fun_signature = {}
      for metric in gauge.gaugeMetricList:
        labels = len(metric.labeldict)
        if(dyn2_fun_signature.get(labels)):
          continue
        constructor_args=""
        gauge_create_labels = "{"
        dynamic_function_call_args=""
        for index in range(labels):
          constructor_args += "std::string label_k"+ str(index) + ", std::string label_v"+ str(index) + ","
          gauge_create_labels += "{" + "label_k"+ str(index) +", label_v" + str(index) + "}"+","
          dynamic_function_call_args += "label_k" + str(index)+", label_v"+str(index) + "," 
        constructor_args += "std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1"
        gauge_create_labels += "{dlabel_k0, dlabel_v0}, {dlabel_k1, dlabel_v1}"
        dynamic_function_call_args += "dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1"
        gauge_create_labels += "}"
        dyn2_fun_signature[labels] = [constructor_args, gauge_create_labels, dynamic_function_call_args]

      for f in dyn2_fun_signature.values():
        constructor_args = f[0]
        gauge_create_labels = f[1]
        dynamic_function_call_args = f[2]
        fh.write("\t\t" + gauge.family + "_DynamicMetricObject2(Family<Gauge> &" + gauge.promfamily + "," + constructor_args +"):\n")
        fh.write("\t\t DynamicMetricObject(),\n")
        fh.write("\t\t gauge(" + gauge.promfamily + ".Add(" + gauge_create_labels + "))\n") 
        fh.write("\t\t{\n")
        fh.write("\t\t}\n")

      fh.write("\t\t~" + gauge.family + "_DynamicMetricObject2" + "()\n")
      fh.write("\t\t{\n")
      fh.write("\t\t}\n")
      fh.write("\t\tGauge &gauge;\n")
      fh.write("};\n")
      #end of 2 dynamic arg 
      #3 dynamic label 

      fh.write("\n\n")
      fh.write("class " + gauge.family + "_DynamicMetricObject3 : public DynamicMetricObject" + " {\n")
      fh.write("\tpublic:\n")
      fh.write("\n\n")
      dyn3_fun_signature = {}
      for metric in gauge.gaugeMetricList:
        labels = len(metric.labeldict)
        if(dyn3_fun_signature.get(labels)):
          continue
        constructor_args=""
        gauge_create_labels = "{"
        dynamic_function_call_args=""
        for index in range(labels):
          constructor_args += "std::string label_k"+ str(index) + ", std::string label_v"+ str(index) + ","
          gauge_create_labels += "{" + "label_k"+ str(index) +", label_v" + str(index) + "}"+","
          dynamic_function_call_args += "label_k" + str(index)+", label_v"+str(index) + "," 
        constructor_args += "std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1, std::string dlabel_k2, std::string dlabel_v2"
        gauge_create_labels += "{dlabel_k0, dlabel_v0}, {dlabel_k1, dlabel_v1}, {dlabel_k2, dlabel_v2}"
        dynamic_function_call_args += "dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1, dlabel_k2, dlabel_v2"
        gauge_create_labels += "}"
        dyn3_fun_signature[labels] = [constructor_args, gauge_create_labels, dynamic_function_call_args]

      for f in dyn3_fun_signature.values():
        constructor_args = f[0]
        gauge_create_labels = f[1]
        dynamic_function_call_args = f[2]
        fh.write("\t\t" + gauge.family + "_DynamicMetricObject3(Family<Gauge> &" + gauge.promfamily + "," + constructor_args +"):\n")
        fh.write("\t\t DynamicMetricObject(),\n")
        fh.write("\t\t gauge(" + gauge.promfamily + ".Add(" + gauge_create_labels + "))\n") 
        fh.write("\t\t{\n")
        fh.write("\t\t}\n")

      fh.write("\t\t~" + gauge.family + "_DynamicMetricObject3" + "()\n")
      fh.write("\t\t{\n")
      fh.write("\t\t}\n")
      fh.write("\t\tGauge &gauge;\n")
      fh.write("};\n")
      #end of 3 dynamic arg 

      fh.write("class " + gauge.classname + " {\n")
      fh.write("\tpublic:\n")
      fh.write("\t" + gauge.classname + "();\n")
      fh.write("\t~" + gauge.classname + "();\n")
      fh.write("\tFamily<Gauge> &"+ gauge.promfamily + ";\n")
      for metric in gauge.gaugeMetricList:
        fh.write("\tGauge &" + metric.gauge_name + ";\n")

      for f in dyn1_fun_signature.values():
        constructor_args,gauge_create_labels,dynamic_function_call_args = f
        fh.write("\n\t" + gauge.family+"_DynamicMetricObject1* add_dynamic1(" + constructor_args + ") {\n")
        fh.write("\t\treturn new "+ gauge.family+"_"+"DynamicMetricObject1("+ gauge.promfamily + "," + dynamic_function_call_args + ");\n ")
        fh.write("\t}\n")


      for f in dyn2_fun_signature.values():
        constructor_args,gauge_create_labels,dynamic_function_call_args = f
        fh.write("\n\t" + gauge.family+"_DynamicMetricObject2* add_dynamic2(" + constructor_args + ") {\n")
        fh.write("\t\treturn new "+ gauge.family+"_"+"DynamicMetricObject2("+ gauge.promfamily + "," + dynamic_function_call_args + ");\n ")
        fh.write("\t}\n")


      for f in dyn3_fun_signature.values():
        constructor_args,gauge_create_labels,dynamic_function_call_args = f
        fh.write("\n\t" + gauge.family+"_DynamicMetricObject3* add_dynamic3(" + constructor_args + ") {\n")
        fh.write("\t\treturn new "+ gauge.family+"_"+"DynamicMetricObject3("+ gauge.promfamily + "," + dynamic_function_call_args + ");\n ")
        fh.write("\t}\n")

      fh.write("};\n\n\n")


def add_counter_classes(fh):
    for counter in counter_family_object_list:
      fh.write("\n\n")
      #start single dynamic label 1
      fh.write("class " + counter.family + "_DynamicMetricObject1 : public DynamicMetricObject" + " {\n")
      fh.write("\tpublic:\n")

      dyn1_fun_signature = {}
      for metric in counter.counterMetricList:
        labels = len(metric.labeldict)
        if(dyn1_fun_signature.get(labels)):
          continue
        constructor_args=""
        counter_create_labels = "{"
        dynamic_function_call_args=""
        for index in range(labels):
          constructor_args += "std::string label_k"+ str(index) + ",std::string label_v"+ str(index) + ","
          counter_create_labels += "{" + "label_k"+ str(index) +", label_v" + str(index) + "}"+","
          dynamic_function_call_args += "label_k" + str(index)+",label_v"+str(index) + "," 
        constructor_args += "std::string dlabel_k0, std::string dlabel_v0"
        counter_create_labels += "{dlabel_k0, dlabel_v0}"
        dynamic_function_call_args += "dlabel_k0, dlabel_v0"
        counter_create_labels += "}"
        dyn1_fun_signature[labels] = [constructor_args, counter_create_labels, dynamic_function_call_args]


      for f in dyn1_fun_signature.values():
        constructor_args = f[0]
        counter_create_labels = f[1]
        dynamic_function_call_args = f[2]
        fh.write("\t\t" + counter.family + "_DynamicMetricObject1(Family<Counter> &" + counter.promfamily + "," + constructor_args +"):\n")
        fh.write("\t\t DynamicMetricObject(),\n")
        fh.write("\t\t counter(" + counter.promfamily + ".Add(" + counter_create_labels + "))\n") 
        fh.write("\t\t{\n")
        fh.write("\t\t}\n")

      fh.write("\t\t~" + counter.family + "_DynamicMetricObject1" + "()\n")
      fh.write("\t\t{\n")
      fh.write("\t\t}\n")
      fh.write("\t\tCounter &counter;\n")
      fh.write("};\n")
      #end single dynamic label 1

      #start single dynamic label 2
      fh.write("\n\n")
      fh.write("class " + counter.family + "_DynamicMetricObject2 : public DynamicMetricObject" + " {\n")
      fh.write("\tpublic:\n")

      dyn2_fun_signature = {}
      for metric in counter.counterMetricList:
        labels = len(metric.labeldict)
        if(dyn2_fun_signature.get(labels)):
          continue
        constructor_args=""
        counter_create_labels = "{"
        dynamic_function_call_args=""
        for index in range(labels):
          constructor_args += "std::string label_k"+ str(index) + ",std::string label_v"+ str(index) + ","
          counter_create_labels += "{" + "label_k"+ str(index) +", label_v" + str(index) + "}"+","
          dynamic_function_call_args += "label_k" + str(index)+",label_v"+str(index) + "," 
        constructor_args += "std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1"
        counter_create_labels += "{dlabel_k0, dlabel_v0}, {dlabel_k1, dlabel_v1}"
        dynamic_function_call_args += "dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1"
        counter_create_labels += "}"
        dyn2_fun_signature[labels] = [constructor_args, counter_create_labels, dynamic_function_call_args]


      for f in dyn2_fun_signature.values():
        constructor_args = f[0]
        counter_create_labels = f[1]
        dynamic_function_call_args = f[2]
        fh.write("\t\t" + counter.family + "_DynamicMetricObject2(Family<Counter> &" + counter.promfamily + "," + constructor_args +"):\n")
        fh.write("\t\t DynamicMetricObject(),\n")
        fh.write("\t\t counter(" + counter.promfamily + ".Add(" + counter_create_labels + "))\n") 
        fh.write("\t\t{\n")
        fh.write("\t\t}\n")

      fh.write("\t\t~" + counter.family + "_DynamicMetricObject2" + "()\n")
      fh.write("\t\t{\n")
      fh.write("\t\t}\n")
      fh.write("\t\tCounter &counter;\n")
      fh.write("};\n")
      #end single dynamic label 2

      #start single dynamic label 3
      fh.write("\n\n")
      fh.write("class " + counter.family + "_DynamicMetricObject3 : public DynamicMetricObject" + " {\n")
      fh.write("\tpublic:\n")

      dyn3_fun_signature = {}
      for metric in counter.counterMetricList:
        labels = len(metric.labeldict)
        if(dyn3_fun_signature.get(labels)):
          continue
        constructor_args=""
        counter_create_labels = "{"
        dynamic_function_call_args=""
        for index in range(labels):
          constructor_args += "std::string label_k"+ str(index) + ",std::string label_v"+ str(index) + ","
          counter_create_labels += "{" + "label_k"+ str(index) +", label_v" + str(index) + "}"+","
          dynamic_function_call_args += "label_k" + str(index)+",label_v"+str(index) + "," 
        constructor_args += "std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2"
        counter_create_labels += "{dlabel_k0, dlabel_v0},{dlabel_k1, dlabel_v1},{dlabel_k2, dlabel_v2}"
        dynamic_function_call_args += "dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1,dlabel_k2, dlabel_v2"
        counter_create_labels += "}"
        dyn3_fun_signature[labels] = [constructor_args, counter_create_labels, dynamic_function_call_args]


      for f in dyn3_fun_signature.values():
        constructor_args = f[0]
        counter_create_labels = f[1]
        dynamic_function_call_args = f[2]
        fh.write("\t\t" + counter.family + "_DynamicMetricObject3(Family<Counter> &" + counter.promfamily + "," + constructor_args +"):\n")
        fh.write("\t\t DynamicMetricObject(),\n")
        fh.write("\t\t counter(" + counter.promfamily + ".Add(" + counter_create_labels + "))\n") 
        fh.write("\t\t{\n")
        fh.write("\t\t}\n")

      fh.write("\t\t~" + counter.family + "_DynamicMetricObject3" + "()\n")
      fh.write("\t\t{\n")
      fh.write("\t\t}\n")
      fh.write("\t\tCounter &counter;\n")
      fh.write("};\n")
      #end single dynamic label 3



      fh.write("class " + counter.classname + " {\n")
      fh.write("\tpublic:\n")
      fh.write("\t" + counter.classname + "();\n")
      fh.write("\t~" + counter.classname + "();\n")
      fh.write("\tFamily<Counter> &"+ counter.promfamily + ";\n")
      for metric in counter.counterMetricList:
        fh.write("\tCounter &" + metric.counter_name + ";\n")

      for f in dyn1_fun_signature.values():
        constructor_args,counter_create_labels,dynamic_function_call_args = f
        fh.write("\n\t" + counter.family+"_DynamicMetricObject1* add_dynamic1(" + constructor_args + ") {\n")
        fh.write("\t\treturn new "+ counter.family+"_"+"DynamicMetricObject1("+ counter.promfamily + "," + dynamic_function_call_args + ");\n ")
        fh.write("\t}\n")

      for f in dyn2_fun_signature.values():
        constructor_args,counter_create_labels,dynamic_function_call_args = f
        fh.write("\n\t" + counter.family+"_DynamicMetricObject2* add_dynamic2(" + constructor_args + ") {\n")
        fh.write("\t\treturn new "+ counter.family+"_"+"DynamicMetricObject2("+ counter.promfamily + "," + dynamic_function_call_args + ");\n ")
        fh.write("\t}\n")

      for f in dyn3_fun_signature.values():
        constructor_args,counter_create_labels,dynamic_function_call_args = f
        fh.write("\n\t" + counter.family+"_DynamicMetricObject3* add_dynamic3(" + constructor_args + ") {\n")
        fh.write("\t\treturn new "+ counter.family+"_"+"DynamicMetricObject3("+ counter.promfamily + "," + dynamic_function_call_args + ");\n ")
        fh.write("\t}\n")



      fh.write("};\n")
      fh.write("\n")

def add_module_stats_class_declaration(fh):
    fh.write("class " + module_name + " {\n")
    fh.write("\t public:\n")
    fh.write("\t\t" + module_name + "();\n")
    fh.write("\t\t~" + module_name + "() {}\n")
    fh.write("\t\tstatic " + module_name + "* Instance(); \n")
    fh.write("\t\tvoid " + module_name + "promThreadSetup(void);\n")
    fh.write("\t\tvoid increment("+module_name+"Counter name, std::map<std::string, std::string> labels={}); \n")
    fh.write("\t\tvoid decrement("+module_name+"Counter name, std::map<std::string, std::string> labels={}); \n")
    fh.write("\t public:\n")
    for gauge in gauges_family_object_list:
      fh.write("\t\t" + gauge.classname + " *" + gauge.moduleStatsMember + ";\n")
    for counter in counter_family_object_list:
      fh.write("\t\t" + counter.classname + " *" + counter.moduleStatsMember +";\n")

    fh.write("\t\tstd::unordered_map<struct Node, DynamicMetricObject*, hash_fn> metrics_map;\n")
    fh.write("};\n")

def create_header_file():
    fname = module_name + "PromClient.h"
    header_file = open(fname, 'w')
    add_copyright(header_file)
    add_header_macro(header_file)
    add_prom_headers(header_file)
    add_namespace_variables(header_file)
    add_setup_function(header_file)
    add_counter_enum(header_file)
    add_dynamic_node_hash_functions(header_file);
    add_gauge_classes(header_file)
    add_counter_classes(header_file)
    add_module_stats_class_declaration(header_file)
    add_footer_macro(header_file)

def add_stl_header(fh):
    fh.write("#include <iostream>\n")
    fh.write("#include <chrono>\n")
    fh.write("#include <map>\n")
    fh.write("#include <memory>\n")
    fh.write("#include <thread>\n")
    fh.write("#include <sstream>\n")
    fh.write("#include \"" + module_name + "PromClient.h\"\n") 
    fh.write("\n") 
    fh.write("using namespace prometheus;\n")
    fh.write("std::shared_ptr<Registry> registry;\n")
    fh.write("\n") 

def add_prom_client_setup_function(fh):
    fname = module_name + "SetupPrometheusThread"
    fh.write("void " + fname + "(uint16_t port)\n")
    fh.write("{\n")
    fh.write("    std::stringstream ss;\n");
    fh.write("    ss << \"0.0.0.0\"<<\":\"<<port;\n");
    fh.write("    registry = std::make_shared<Registry>();\n")
    fh.write("    /* Create single instance */ \n")
    fh.write("    " + module_name + "::Instance(); \n")
    fh.write("    Exposer exposer{ss.str(), 1};\n")
    fh.write("    std::string metrics(\"/metrics\");\n")
    fh.write("    exposer.RegisterCollectable(registry, metrics);\n")
    fh.write("    while(1)\n")
    fh.write("    {\n")
    fh.write("        std::this_thread::sleep_for(std::chrono::seconds(1));\n")
    fh.write("    }\n")
    fh.write("}\n")

def add_module_stats_class_defination(fh):
    fh.write(module_name + "::" + module_name + "()\n")
    fh.write("{\n")
    for gauge in gauges_family_object_list:
      fh.write("\t " + gauge.moduleStatsMember  + " = new " + gauge.classname + ";\n")
    for counter in counter_family_object_list:
      fh.write("\t " + counter.moduleStatsMember + " = new " + counter.classname + ";\n")
  
    fh.write("}\n")

def add_singleton_obect_create(fh):
    fh.write(module_name + "* " + module_name + "::" + "Instance() \n")
    fh.write("{\n")
    fh.write("\tstatic " + module_name + " object;\n")
    fh.write("\treturn &object; \n")
    fh.write("}\n")
    
def add_gauge_class_defination(fh):
    for gauge in gauges_family_object_list:
      fh.write("\n\n")
      fh.write(gauge.classname + "::" + gauge.classname + "():\n")
      print("family labels {}".format(gauge.family_labels))
      fh.write(gauge.promfamily + "(BuildGauge().Name(\""+ gauge.familyname +"\").Help(\"" + gauge.familyhelp + "\").Labels(" + gauge.family_labels + ").Register(*registry))," + "\n")
      metrics_len = len(gauge.gaugeMetricList)
      for metric_index in range(metrics_len - 1):
        metric = gauge.gaugeMetricList[metric_index]
        fh.write(metric.gauge_name + "(" + gauge.promfamily + ".Add(" + metric.gauge_labels + ")),\n")

      metric = gauge.gaugeMetricList[metrics_len -1]
      fh.write(metric.gauge_name + "(" + gauge.promfamily + ".Add(" + metric.gauge_labels + "))\n")
      
      fh.write("{\n")
      fh.write("}\n")
      fh.write("\n\n")
      fh.write(gauge.classname + "::~" + gauge.classname + "()\n")
      fh.write("{\n")
      fh.write("}\n")
      fh.write("\n\n")

def add_counter_class_defination(fh):
    for counter in counter_family_object_list:
      print("family labels {}".format(counter.family_labels))
      fh.write("\n\n")
      fh.write(counter.classname + "::" + counter.classname + "():\n")
      fh.write(counter.promfamily + "(BuildCounter().Name(\""+ counter.familyname +"\").Help(\"" + counter.familyhelp + "\").Labels(" + counter.family_labels + ").Register(*registry))," + "\n")
      metrics_len = len(counter.counterMetricList)
      metric_final_label = ".Add({"
      found = False
      for metric_index in range(metrics_len - 1):
        metric = counter.counterMetricList[metric_index]
        fh.write(metric.counter_name + "(" + counter.promfamily + ".Add(" + metric.counter_labels + ")),\n")

      if(metrics_len>=1):
        metric_final_label = ".Add({"
        metric = counter.counterMetricList[metrics_len - 1 ]
        fh.write(metric.counter_name + "(" + counter.promfamily + ".Add(" + metric.counter_labels + "))\n")
      
      fh.write("{\n")
      fh.write("}\n")
      fh.write("\n\n")
      fh.write(counter.classname + "::~" + counter.classname + "()\n")
      fh.write("{\n")
      fh.write("}\n")
      fh.write("\n\n")

def add_increment_api(fh):
    fh.write("\n\n")
    fh.write("void " + module_name + "::" + "increment("+module_name+"Counter name,std::map<std::string,std::string> labels)\n") 
    fh.write("{\n")
    fh.write("\tswitch(name) {\n")
    for gauge in gauges_family_object_list:
      for metric in gauge.gaugeMetricList:
        labels = ""
        index = 0
        for l in metric.labeldict:
          if index != 0:
            labels += ","
          for k in l:
             v = l[k]
             labels += "\"" + k + "\"" + ",\"" + v + "\"" 
          index += 1

        fh.write("\tcase "+module_name+"Counter::" + metric.enum_name + ":\n")
        fh.write("\t{\n")
        fh.write("\t\t" + gauge.moduleStatsMember + "->" + metric.gauge_name + ".Increment();\n")
        fh.write("\t\tif(labels.size() == 0) {\n")
        fh.write("\t\tbreak;\n")
        fh.write("\t\t}\n")
        fh.write("\t\tif(labels.size() == 1) {\n")
        fh.write("\t\tauto it = labels. begin();\n")
        fh.write("\t\tstruct Node s1 = {name, it->first, it->second};\n")
        fh.write("\t\tauto it1 = metrics_map.find(s1);\n")
        fh.write("\t\tif(it1 != metrics_map.end()) {\n")
        fh.write("\t\t    "+gauge.family +"_DynamicMetricObject1 *obj = static_cast<"+gauge.family+"_DynamicMetricObject1 *>(it1->second);\n")
        fh.write("\t\t    obj->gauge.Increment();\n")
        fh.write("\t\t} else {\n")
        fh.write("\t\t    "+gauge.family+"_DynamicMetricObject1 *obj = " + gauge.moduleStatsMember + "->add_dynamic1(" + labels + ",it->first, it->second);\n")
        fh.write("\t\t    auto p1 = std::make_pair(s1, obj);\n")
        fh.write("\t\t    metrics_map.insert(p1);\n")
        fh.write("\t\t    obj->gauge.Increment();\n")
        fh.write("\t\t}\n")
        fh.write("\t\t} else if (labels.size() == 2) {\n")
        fh.write("\t\tauto it1 = labels. begin();\n")
        fh.write("\t\tauto it2 = it1++;\n")
        fh.write("\t\tstruct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};\n")
        fh.write("\t\tauto itf = metrics_map.find(s1);\n")
        fh.write("\t\tif(itf != metrics_map.end()) {\n")
        fh.write("\t\t    "+gauge.family +"_DynamicMetricObject2 *obj = static_cast<"+gauge.family+"_DynamicMetricObject2 *>(itf->second);\n")
        fh.write("\t\t    obj->gauge.Increment();\n")
        fh.write("\t\t} else {\n")
        fh.write("\t\t    "+gauge.family+"_DynamicMetricObject2 *obj = " + gauge.moduleStatsMember + "->add_dynamic2(" + labels + ",it1->first, it1->second, it2->first, it2->second);\n")
        fh.write("\t\t    auto p1 = std::make_pair(s1, obj);\n")
        fh.write("\t\t    metrics_map.insert(p1);\n")
        fh.write("\t\t    obj->gauge.Increment();\n")
        fh.write("\t\t} \n")
        fh.write("\t\t} else if (labels.size() == 3) {\n")
        fh.write("\t\tauto it1 = labels. begin();\n")
        fh.write("\t\tauto it2 = it1++;\n")
        fh.write("\t\tauto it3 = it2++;\n")
        fh.write("\t\tstruct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};\n")
        fh.write("\t\tauto itf = metrics_map.find(s1);\n")
        fh.write("\t\tif(itf != metrics_map.end()) {\n")
        fh.write("\t\t    "+gauge.family +"_DynamicMetricObject3 *obj = static_cast<"+gauge.family+"_DynamicMetricObject3 *>(itf->second);\n")
        fh.write("\t\t    obj->gauge.Increment();\n")
        fh.write("\t\t} else {\n")
        fh.write("\t\t    "+gauge.family+"_DynamicMetricObject3 *obj = " + gauge.moduleStatsMember + "->add_dynamic3(" + labels + ",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);\n")
        fh.write("\t\t    auto p1 = std::make_pair(s1, obj);\n")
        fh.write("\t\t    metrics_map.insert(p1);\n")
        fh.write("\t\t    obj->gauge.Increment();\n")
        fh.write("\t\t}\n")
        fh.write("\t\t}\n")
        fh.write("\t\tbreak;\n")
        fh.write("\t}\n")

    for counter in counter_family_object_list:
      for metric in counter.counterMetricList:
        labels = ""
        index = 0
        for l in metric.labeldict:
          if index != 0:
            labels += ","
          for k in l:
             v = l[k]
             labels += "\"" + k + "\"" + ",\"" + v + "\"" 
          index += 1

        fh.write("\tcase "+module_name+"Counter::" + metric.enum_name + ":\n")
        fh.write("\t{\n")
        fh.write("\t\t" + counter.moduleStatsMember+ "->" + metric.counter_name + ".Increment();\n")
        fh.write("\t\tif(labels.size() == 0) {\n")
        fh.write("\t\tbreak;\n")
        fh.write("\t\t}\n")
        fh.write("\t\tif(labels.size() == 1) {\n")
        fh.write("\t\tauto it = labels. begin();\n")
        fh.write("\t\tstruct Node s1 = {name, it->first, it->second};\n")
        fh.write("\t\tauto it1 = metrics_map.find(s1);\n")
        fh.write("\t\tif(it1 != metrics_map.end()) {\n")
        fh.write("\t\t    "+ counter.family+ "_DynamicMetricObject1 *obj = static_cast<" + counter.family+ "_DynamicMetricObject1 *>(it1->second);\n")
        fh.write("\t\t    obj->counter.Increment();\n")
        fh.write("\t\t} else {\n")
        fh.write("\t\t    "+counter.family + "_DynamicMetricObject1 *obj = " + counter.moduleStatsMember + "->add_dynamic1(" + labels + ",it->first, it->second);\n")
        fh.write("\t\t    auto p1 = std::make_pair(s1, obj);\n")
        fh.write("\t\t    metrics_map.insert(p1);\n")
        fh.write("\t\t    obj->counter.Increment();\n")
        fh.write("\t\t}\n")
        fh.write("\t\t} else if (labels.size() == 2) {\n")
        fh.write("\t\tauto it1 = labels. begin();\n")
        fh.write("\t\tauto it2 = it1++;\n")
        fh.write("\t\tstruct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};\n")
        fh.write("\t\tauto itf = metrics_map.find(s1);\n")
        fh.write("\t\tif(itf != metrics_map.end()) {\n")
        fh.write("\t\t    "+counter.family +"_DynamicMetricObject2 *obj = static_cast<"+counter.family+"_DynamicMetricObject2 *>(itf->second);\n")
        fh.write("\t\t    obj->counter.Increment();\n")
        fh.write("\t\t} else {\n")
        fh.write("\t\t    "+counter.family+"_DynamicMetricObject2 *obj = " + counter.moduleStatsMember + "->add_dynamic2(" + labels + ",it1->first, it1->second, it2->first, it2->second);\n")
        fh.write("\t\t    auto p1 = std::make_pair(s1, obj);\n")
        fh.write("\t\t    metrics_map.insert(p1);\n")
        fh.write("\t\t    obj->counter.Increment();\n")
        fh.write("\t\t} \n")
        fh.write("\t\t} else if (labels.size() == 3) {\n")
        fh.write("\t\tauto it1 = labels. begin();\n")
        fh.write("\t\tauto it2 = it1++;\n")
        fh.write("\t\tauto it3 = it2++;\n")
        fh.write("\t\tstruct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};\n")
        fh.write("\t\tauto itf = metrics_map.find(s1);\n")
        fh.write("\t\tif(itf != metrics_map.end()) {\n")
        fh.write("\t\t    "+counter.family +"_DynamicMetricObject3 *obj = static_cast<"+counter.family+"_DynamicMetricObject3 *>(itf->second);\n")
        fh.write("\t\t    obj->counter.Increment();\n")
        fh.write("\t\t} else {\n")
        fh.write("\t\t    "+counter.family+"_DynamicMetricObject3 *obj = " + counter.moduleStatsMember + "->add_dynamic3(" + labels + ",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);\n")
        fh.write("\t\t    auto p1 = std::make_pair(s1, obj);\n")
        fh.write("\t\t    metrics_map.insert(p1);\n")
        fh.write("\t\t    obj->counter.Increment();\n")
        fh.write("\t\t}\n")
        fh.write("\t\t}\n")
        fh.write("\t\tbreak;\n")
        fh.write("\t}\n")
    fh.write("\tdefault:\n");
    fh.write("\t\tbreak;\n");
    fh.write("\t}\n")
    fh.write("}\n")
    fh.write("\n\n")
      
def add_decrement_api(fh):
    fh.write("\n\n")
    fh.write("void " + module_name + "::" + "decrement("+module_name+"Counter name,std::map<std::string,std::string> labels)\n") 
    fh.write("{\n")
    fh.write("\tswitch(name) {\n")
    for gauge in gauges_family_object_list:
      for metric in gauge.gaugeMetricList:
        labels = ""
        index = 0
        for l in metric.labeldict:
          if index != 0:
            labels += ","
          for k in l:
             v = l[k]
             labels += "\"" + k + "\"" + ",\"" + v + "\"" 
          index += 1

        fh.write("\tcase "+module_name+"Counter::" + metric.enum_name + ":\n")
        fh.write("\t{\n")
        fh.write("\t\t" + gauge.moduleStatsMember + "->" + metric.gauge_name + ".Decrement();\n")
        fh.write("\t\tif(labels.size() == 0) {\n")
        fh.write("\t\tbreak;\n")
        fh.write("\t\t}\n")
        fh.write("\t\tif(labels.size() == 1) {\n")
        fh.write("\t\tauto it = labels. begin();\n")
        fh.write("\t\tstruct Node s1 = {name, it->first, it->second};\n")
        fh.write("\t\tauto it1 = metrics_map.find(s1);\n")
        fh.write("\t\tif(it1 != metrics_map.end()) {\n")
        fh.write("\t\t    "+gauge.family +"_DynamicMetricObject1 *obj = static_cast<"+gauge.family+"_DynamicMetricObject1 *>(it1->second);\n")
        fh.write("\t\t    obj->gauge.Decrement();\n")
        fh.write("\t\t}\n")
        fh.write("\t\t} else if (labels.size() == 2) {\n")
        fh.write("\t\tauto it1 = labels. begin();\n")
        fh.write("\t\tauto it2 = it1++;\n")
        fh.write("\t\tstruct Node s1 = {name, it1->first+it2->first, it1->second+it2->second};\n")
        fh.write("\t\tauto itf = metrics_map.find(s1);\n")
        fh.write("\t\tif(itf != metrics_map.end()) {\n")
        fh.write("\t\t    "+gauge.family +"_DynamicMetricObject2 *obj = static_cast<"+gauge.family+"_DynamicMetricObject2 *>(itf->second);\n")
        fh.write("\t\t    obj->gauge.Decrement();\n")
        fh.write("\t\t} \n")
        fh.write("\t\t} else if (labels.size() == 3) {\n")
        fh.write("\t\tauto it1 = labels. begin();\n")
        fh.write("\t\tauto it2 = it1++;\n")
        fh.write("\t\tauto it3 = it2++;\n")
        fh.write("\t\tstruct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};\n")
        fh.write("\t\tauto itf = metrics_map.find(s1);\n")
        fh.write("\t\tif(itf != metrics_map.end()) {\n")
        fh.write("\t\t    "+gauge.family +"_DynamicMetricObject3 *obj = static_cast<"+gauge.family+"_DynamicMetricObject3 *>(itf->second);\n")
        fh.write("\t\t    obj->gauge.Decrement();\n")
        fh.write("\t\t}\n")
        fh.write("\t\t}\n")
        fh.write("\t\tbreak;\n")
        fh.write("\t}\n")

    fh.write("\tdefault:\n");
    fh.write("\t\tbreak;\n");
    fh.write("\t}\n")
    fh.write("}\n")
    fh.write("\n\n")
 
def add_test_main_function(fh):
    fh.write("#ifdef TEST_PROMETHEUS \n")
    fh.write("#include <unistd.h>\n")
    fh.write("int main() {\n")
    fh.write("\tstd::thread prom(" + module_name + "SetupPrometheusThread, 3081);\n")
    fh.write("\tprom.detach();\n")
    fh.write("\tsleep(1);\n")
    fh.write("\twhile(1) {\n")
    fh.write("\tspgwStats::Instance()->increment(spgwStatsCounter::NUM_UE_SPGW_ACTIVE_SUBSCRIBERS, {{\"mme_addr\",\"1.1.1.1\"},{\"spgwu_addr\", \"1.1.1.2\"}});\n") 
    fh.write("\tsleep(1);\n")
    fh.write("\t}\n")
    fh.write("}\n")
    fh.write("#endif\n")

def create_cpp_file():
    fname = module_name+"PromClient.cpp"
    cpp_file = open(fname, 'w')
    add_copyright(cpp_file)
    add_stl_header(cpp_file)
    add_prom_client_setup_function(cpp_file);
    add_module_stats_class_defination(cpp_file)
    add_singleton_obect_create(cpp_file)
    add_gauge_class_defination(cpp_file)
    add_counter_class_defination(cpp_file)
    add_increment_api(cpp_file)
    add_decrement_api(cpp_file)
    add_test_main_function(cpp_file)    

def add_c_counter_enum(fh):
    fh.write("\n") 
    fh.write("enum " + module_name + "Enum {\n")
    enum_class = []
    for gauge in gauges_family_object_list:
      for metric in gauge.gaugeMetricList:
        enum_class.append(metric.enum_name)
    for counter in counter_family_object_list:
      for metric in counter.counterMetricList:
        enum_class.append(metric.enum_name)
    x = len(enum_class)
    for i in range(x-1):
        fh.write("\t" + enum_class[i].upper() + ",\n")
    fh.write("\t" + enum_class[x-1].upper() + "\n")
    fh.write("};\n") 
    fh.write("\n") 
 
def create_c_enum_file():
    fname = module_name + "PromEnum.h"
    header_file = open(fname, 'w')
    add_copyright(header_file)
    h = "#ifndef _INCLUDE_"+module_name+"_ENUM_H__\n"
    header_file.write(h)
    h = "#define _INCLUDE_"+module_name+"_ENUM_H__\n"
    header_file.write(h)
    add_c_counter_enum(header_file)
    header_file.write("\n") 
    h = "#endif /* _INCLUDE_"+module_name+"_ENUM_H__ */\n"
    header_file.write(h)



parse_json_file()
create_header_file()
create_cpp_file()
create_c_enum_file()

