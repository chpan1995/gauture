<template>
  <div class="main_contain">
    <el-button
      type="primary"
      style="width: 100px"
      @click="addGrainType"
      :disabled="isEdit"
    >
      新增
    </el-button>
    <el-card class="contain">
      <template v-for="(item, index) in graintypeList" :key="index">
        <div class="lab">
          <span style="margin-right: 10px; width: 40px">种类</span>
          <el-input
            class="elinput"
            :input-style="{ textAlign: 'center' }"
            v-model="graintypeList[index]"
            :disabled="!isEdit"
          ></el-input>
        </div>
      </template>
      <div class="tmp" v-if="isAdd">
        <span style="margin-right: 10px; width: 40px">种类</span>
        <el-input
          class="elinput"
          :input-style="{ textAlign: 'center' }"
          v-model="newGraintype"
          style="width: 360px; height: 40px"
        ></el-input>
      </div>
      <div>
        <el-button type="primary" style="width: 100px" @click="saveGrainType">
          保存
        </el-button>
        <el-button
          type="primary"
          style="width: 100px"
          @click="editGrainType"
          :disabled="isAdd"
        >
          编辑
        </el-button>
      </div>
    </el-card>
  </div>
</template>

<script setup lang="ts">
import { ref } from "vue";
import { ElMessageBox } from "element-plus";
const graintypeList = ref<string[]>(["小麦", "玉米"]);

const isAdd = ref<boolean>(false);
const isEdit = ref<boolean>(false);

const newGraintype = ref<string>("");

const addGrainType = () => {
  isAdd.value = true;
};

const saveGrainType = () => {
  console.log(newGraintype.value);
  if (isAdd.value) {
    if (newGraintype.value == "") {
      ElMessageBox.alert("输入不能为空", { confirmButtonText: "确定" });
    } else {
      isAdd.value = false;
      graintypeList.value.push(newGraintype.value);
      newGraintype.value = "";
    }
    return;
  }

  if (isEdit.value) {
    graintypeList.value.forEach((element) => {
      if (element == "") {
        ElMessageBox.alert("输入不能为空", { confirmButtonText: "确定" });
      }
      return;
    });
    isEdit.value = false;
  }
};

const editGrainType = () => {
  isEdit.value = true;
};
</script>

<style lang="scss" scoped>
.contain {
  margin-top: 20px;
  .lab {
    display: flex;
    margin-bottom: 20px;
    align-items: center;
    .elinput {
      width: 360px;
      height: 40px;

      /* Style the disabled input using the correct selectors */
      :deep(.el-input__wrapper.is-disabled) {
        background-color: #1a98d6 !important;
        box-shadow: 0 0 0 1px #cbcbcb inset !important;
      }

      :deep(.el-input__wrapper.is-disabled .el-input__inner) {
        color: #333333 !important;
        -webkit-text-fill-color: #333333 !important;
        cursor: not-allowed;
      }
    }
  }
  .tmp {
    display: flex;
    align-items: center;
    margin-bottom: 20px;
  }
}
</style>