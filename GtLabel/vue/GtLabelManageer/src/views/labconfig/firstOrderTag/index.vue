<template>
  <div class="main">
    <div
      style="display: flex; align-items: center; gap: 10px; margin-bottom: 24px"
    >
      <p>种类：</p>
      <el-select v-model="selectValue" size="large" style="width: 240px">
        <el-option
          v-for="item in options"
          :key="item.value"
          :label="item.label"
          :value="item.value"
        />
      </el-select>
      <el-button type="primary" @click="dialogVisible = true">
        新增配置
      </el-button>
    </div>
    <el-card>
      <template v-for="(item, index) in testone" :key="index">
        <div class="first_type_div">
          <span class="first_type_span"> 一级分类 </span>
          <span class="first_value_span"> {{ item.value }} </span>
        </div>
        <div class="first_tags">
          <template v-for="(item, index) in item.items" :key="index">
            <div
              style="
                display: flex;
                align-items: center;
                gap: 10px;
                flex-wrap: wrap;
              "
            >
              <span class="first_tags_span"> 一级标签 </span>
              <span class="first_tags_value"> {{ item.value }} </span>
            </div>
          </template>
        </div>
        <!-- 分割线，避免最后一个元素也有分割线 -->
        <hr v-if="index !== testone.length - 1" class="divider" />
      </template>
    </el-card>

    <el-dialog
      v-model="dialogVisible"
      title="新增配置"
      width="900"
      style="height: 700px"
      @close="resetData"
    >
      <div class="dia" style="height: 630px; overflow: auto">
        <input
          type="file"
          ref="fileInputRef"
          @change="handleFileUpload"
          accept=".json"
        />
        <el-input
          v-model="fileValue"
          style="width: 100%; margin-top: 10px; margin-bottom: 10px"
          :rows="2"
          type="textarea"
          placeholder="Please input"
          resize="none"
        >
        </el-input>
        <!-- 这里使用 Flexbox 让按钮靠右 -->
        <div
          style="
            display: flex;
            justify-content: flex-end;
            margin-top: 10px;
            margin-bottom: 30px;
          "
        >
          <el-button type="primary" @click="parseJson">解析</el-button>
        </div>
        <template v-for="(item, index) in tags" :key="index">
          <div class="first_type_div" v-if="item.value">
            <span class="first_type_span"> 一级分类 </span>
            <span class="first_value_span"> {{ item.value }} </span>
          </div>
          <div class="first_tags" v-if="item.items">
            <template v-for="(item, index) in item.items" :key="index">
              <div
                style="
                  display: flex;
                  align-items: center;
                  gap: 10px;
                  flex-wrap: wrap;
                "
              >
                <span class="first_tags_span"> 一级标签 </span>
                <span class="first_tags_value"> {{ item.value }} </span>
              </div>
            </template>
          </div>
          <hr v-if="index !== tags.length - 1 && item.value" class="divider" />
          <div
            style="display: flex; justify-content: flex-end; margin-top: 10px"
          >
            <el-button
              v-if="index === tags.length - 1 && item.value"
              type="primary"
            >
              保存
            </el-button>
          </div>
        </template>
      </div>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from "vue";
import { ElMessageBox } from "element-plus";

const selectValue = ref("wheat");
const dialogVisible = ref(false);
const fileValue = ref<string>("");
const tags = ref<any[]>();

const fileInputRef = ref();

const options = [
  {
    value: "wheat",
    label: "小麦",
  },
  {
    value: "corn",
    label: "玉米",
  },
];

const testone = [
  {
    value: "缺陷程度",
    items: [
      {
        value: "重度",
        items: [],
      },
      {
        value: "中度",
        items: [],
      },
      {
        value: "轻微",
        items: [],
      },
    ],
  },
  {
    value: "缺陷区域",
    items: [
      {
        value: "胚乳",
        items: [],
      },
      {
        value: "胚部",
        items: [],
      },
      {
        value: "尾部",
        items: [],
      },
      {
        value: "整体",
        items: [],
      },
    ],
  },
  {
    value: "缺陷形状",
    items: [
      {
        value: "片状",
        items: [],
      },
      {
        value: "斑块",
        items: [],
      },
      {
        value: "点状",
        items: [],
      },
    ],
  },
  {
    value: "颗粒姿态",
    items: [
      {
        value: "侧躺",
        items: [],
      },
    ],
  },
  {
    value: "缺陷颜色",
    items: [
      {
        value: "灰绿色",
        items: [],
      },
      {
        value: "白色",
        items: [],
      },
      {
        value: "黑色",
        items: [],
      },
    ],
  },
  {
    value: "干扰",
    items: [
      {
        value: "杂质",
        items: [],
      },
      {
        value: "有灰",
        items: [],
      },
      {
        value: "多颗粘连",
        items: [],
      },
      {
        value: "异色",
        items: [],
      },
      {
        value: "虫",
        items: [],
      },
      {
        value: "虫",
        items: [],
      },
      {
        value: "虫",
        items: [],
      },
      {
        value: "虫",
        items: [],
      },
      {
        value: "虫",
        items: [],
      },
    ],
  },
];

// let tags = []; 由于 tags 不是 ref 或 reactive，Vue 不会监听它的变化

onMounted(() => {
  console.log(selectValue.value);
});

const handleFileUpload = (event) => {
  const file = event.target.files[0]; // 获取文件
  if (!file) return;

  const reader = new FileReader();

  reader.onload = (e) => {
    fileValue.value = e.target.result as string; // 读取到的文件内容
  };

  reader.readAsText(file); // 以文本格式读取
};

const parseJson = () => {
  try {
    let obj = JSON.parse(fileValue.value);
    if (Array.isArray(obj)) {
      tags.value = obj;
    } else {
      ElMessageBox.alert("内容输入有误", { confirmButtonText: "确定" });
    }
  } catch (error) {
    ElMessageBox.alert("内容输入有误", { confirmButtonText: "确定" });
  }
};

const resetData = () => {
  tags.value = [];
  fileValue.value = "";
  fileInputRef.value.value = "";
};
</script>

<style lang="scss">
.main {
  .first_type_div {
    display: flex;
    align-items: center; /* 垂直居中 */
    gap: 15px; // 控制间距
    margin-bottom: 24px;
    .first_value_span {
      display: flex;
      justify-content: center; /* 水平居中 */
      align-items: center; /* 垂直居中 */
      background: #f1f3f4;
      border-radius: 4px;
      border: 1px solid #cbcbcb;
      width: 224px;
      height: 40px;
      color: #333333;
    }
  }
  .first_type_span {
    font-size: 14px;
    color: #333333;
  }
  .first_tags {
    margin-left: 340px;
    margin-top: -64px;
    display: flex;
    align-items: center; /* 垂直居中 */
    gap: 15px; // 控制间距
    margin-bottom: 24px;
    flex-wrap: wrap;
    .first_tags_span {
      font-size: 14px;
      color: #666666;
    }
    .first_tags_value {
      display: flex;
      justify-content: center; /* 水平居中 */
      align-items: center; /* 垂直居中 */
      background: #f1f3f4;
      border-radius: 4px;
      border: 1px solid #cbcbcb;
      width: 88px;
      height: 40px;
      color: #333333;
    }
  }
  .divider {
    border: none;
    height: 1px;
    background: #e0e0e0;
    margin: 16px 0;
  }
  .el-textarea__inner {
    height: 360px !important;
  }
  //   :deep(.el-dialog__header) {
  //     position: sticky;
  //     top: 0;
  //     background: white;
  //     z-index: 10;
  //     padding: 16px;
  //     box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
  //   }
  /* 让对话框内容部分滚动 */
  //   .el-dialog__body {
  //     overflow: auto; /* 启用滚动 */
  //   }
}
</style>